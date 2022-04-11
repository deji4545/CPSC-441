// Name - Ayodeji Osho
// Class - CPSC 441 T07
// Student ID -30071771

// Server side implementation of UDP client-server model
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 1358
#define MAXLINE 1024

char money[MAXLINE];

int exchange(char *buffer)
{
	char exchange_Name[4][10] = {"USD", "EURO", "GBP", "BITCOIN"};
	float exchange_val[4] = {0.81, 0.70, 0.59, 0.000016};

	// REFERENCE - https://www.tutorialspoint.com/c_standard_library/c_function_strtod.htm
	// Extract the decimal value from client
	char *pointer;
	float ret;
	ret = strtod(buffer, &pointer);

	// REFERENCE - https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
	// split <val> <CAD> <currency>
	char delim[] = " ";
	char *ptr = strtok(buffer, delim);

	ptr = strtok(NULL, delim); // retrieve the desired currency
	ptr = strtok(NULL, delim);

	int i;

	for (i = 0; i < 4; i++)
	{
		if (strcmp(ptr, exchange_Name[i]) == 0)
		{

			float val = ret * exchange_val[i];
			snprintf(money, sizeof(money), "%.4f", val);
			gcvt(val, 4, money);
			break;
		}
	}

	return 0;
}

// Driver code
int main()
{

	char exchange_Name[4][10] = {"USD", "EURO", "GBP", "BITCOIN"};
	float exchange_val[4] = {0.81, 0.70, 0.59, 0.000016};

	int i = 0;
	for (i = 0; i < 4; i++)
	{
		printf("CAD To %s = %f\n", exchange_Name[i], exchange_val[i]);
	}

	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;

	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&servaddr,
			 sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len, n;

	len = sizeof(cliaddr); // len is value/resuslt

	while (1)
	{
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
		buffer[n - 1] = '\0';
		// puts(buffer);

		exchange(buffer);

		sendto(sockfd, (const char *)money, strlen(money), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);

		bzero(money, sizeof(money));
		bzero(buffer, sizeof(buffer));
	}

	return 0;
}
