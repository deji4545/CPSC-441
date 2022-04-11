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

#define PORT 1868
#define MAXLINE 1024

// Driver code
int main()
{

    int encryptionKey = 4;
    char *str_encryptionKey = "4";

    char candidate_names[4][20] = {"Dev Maclean ", "Jillian Hopkins ", "Elis Marks ", "Kevin Hooper "};
    int candidate_id[4] = {1765, 1122, 8653, 5392};
    int candidate_votes[4] = {44, 22, 77, 88};

    char destA[200];
    char destB[1000];
    snprintf(destB, sizeof(destB), "%s %d\n%s %d\n%s %d\n%s %d\n", candidate_names[0], candidate_id[0], candidate_names[1], candidate_id[1], candidate_names[2], candidate_id[2], candidate_names[3], candidate_id[3]);

    printf(destB);

    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
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
        bzero(destA, sizeof(destA));
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        if (atoi(buffer) == 3)
        {
            sendto(sockfd, (const char *)str_encryptionKey, sizeof(str_encryptionKey), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        }
        else
        {

            int encryptedMessage = atoi(buffer);
            int decryption = encryptedMessage / encryptionKey;
            int i;
            for (i = 0; i < 4; i++)
            {
                if (decryption == candidate_id[i])
                {
                    candidate_votes[i] = candidate_votes[i] + 1;
                    break;
                }
            }
            snprintf(destA, sizeof(destA), "%s %d %d\n%s %d %d\n%s %d %d\n%s %d %d\n", candidate_names[0], candidate_id[0], candidate_votes[0], candidate_names[1], candidate_id[1], candidate_votes[1], candidate_names[2], candidate_id[2], candidate_votes[2], candidate_names[3], candidate_id[3], candidate_votes[3]);
            sendto(sockfd, (char *)destA, sizeof(destA), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        }
    }

    return 0;
}
