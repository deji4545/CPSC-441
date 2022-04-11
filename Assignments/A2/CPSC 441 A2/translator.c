// Name - Ayodeji Osho
// Class - CPSC 441 T07
// Student ID -30071771

// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 1982
#define MAXLINE 1024

// Driver code
int main()
{
    // English and French Words
    char english[5][10] = {"hello", "night", "cake", "travel", "fun"};
    char french[6][30] = {"bonjour", "nuit", "gateau", "voyager", "amusant", "Word cannot be translated"};

    printf("Word To Translate\n");
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        printf("%s = %s\n", english[i], french[i]);
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
    int j;

    printf("\nThis translator server is at your service:\n");
    while (1)
    {

        bzero(buffer, sizeof(buffer));
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n - 1] = '\0';

        int i;
        int val = 5;

        // Searches if english word is part of list
        for (i = 0; i < 5; i++)
        {
            if (strcmp(english[i], buffer) == 0)
            {
                val = i;
                break;
            }
        }

        sendto(sockfd, french[val], strlen(french[val]),
               MSG_CONFIRM, (const struct sockaddr *)&cliaddr,
               len);
    }
    return 0;
}
