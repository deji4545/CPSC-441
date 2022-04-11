
// REFERENCE CODE
#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>

//#define PORT 1982
#define MAXLINE 1024
int portNumber;
char micro_Message[MAXLINE];

int microservice(char *translate_word)
{
    int sockfd;

    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(portNumber);
    servaddr.sin_addr.s_addr = inet_addr("136.159.5.25");

    int n, len;

    int value = sendto(sockfd, (const char *)translate_word, strlen(translate_word),
                       MSG_CONFIRM, (const struct sockaddr *)&servaddr,
                       sizeof(servaddr));
    printf("%s message sent\n", translate_word);

    n = recvfrom(sockfd, (char *)micro_Message, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *)&servaddr,
                 &len);

    // micro_Message[n] = '\0';

    printf("Server : %s\n", micro_Message);

    close(sockfd);
    return 0;
}

int indirect()
{

    int socket_desc, client_sock;
    struct sockaddr_in server, client;
    char client_message[5000];

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
    }
    puts("Socket created\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1488); // 1489

    int enable = 1;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
    {
        printf("setsockopt(SO_REUSEADDR) failed");
    }

    // Bind
    int bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
    if (bindStatus == -1)
    {
        // print the error message
        perror("Binding failed");
        return 1;
    }
    printf("Binding done.\n");

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    printf("Waiting for clients...\n");

    // accept connection from an incoming client
    client_sock = accept(socket_desc, NULL, NULL);
    if (client_sock < 0)
    {
        perror("Connection failed");
        return 1;
    }
    printf("Connection accepted\n");

    // Receive a message from client
    char service_Num_Str[5];

    int recvStatus;
    char candidates[1000];
    int someBool = 1;

    while (1)
    {

        recvStatus = recv(client_sock, service_Num_Str, sizeof(service_Num_Str), 0);
        int service_Num = atoi(service_Num_Str);

        if (recvStatus == -1)
        {
            printf("Error in receiving!");
            break;
        }

        if (service_Num == 1)
        {
            portNumber = 1982;
            recv(client_sock, client_message, sizeof(client_message), 0);
            microservice(client_message);
            send(client_sock, micro_Message, sizeof(micro_Message), 0);
        }

        if (service_Num == 2)
        {
            portNumber = 1358;
            recv(client_sock, client_message, sizeof(client_message), 0);
            microservice(client_message);
            send(client_sock, micro_Message, sizeof(micro_Message), 0);
        }

        if (service_Num == 4)
        {
            portNumber = 1868;

            if (someBool == 1)
            {
                microservice(service_Num_Str);
                snprintf(candidates, sizeof(candidates), "%s", micro_Message);
                someBool == 0;
            }
            send(client_sock, candidates, sizeof(candidates), 0);
        }

        if (service_Num == 3)
        {
            portNumber = 1868;
            microservice(service_Num_Str);                              // ask for the encryption key
            send(client_sock, micro_Message, sizeof(micro_Message), 0); // send the encryption key to the client
            bzero(micro_Message, sizeof(micro_Message));                // clear message
            recv(client_sock, client_message, sizeof(client_message), 0);
            // bzero(micro_Message, sizeof(micro_Message));
            microservice(client_message);
            send(client_sock, micro_Message, sizeof(micro_Message), 0);
        }

        // printf("Reach 1\n");
        // recv(client_sock, client_message, sizeof(client_message), 0);
        // printf("This is the client Message: %s\n", client_message);
        // microservice(client_message);
        // printf("Reach 2\n");
        // send(client_sock, micro_Message, sizeof(micro_Message), 0);

        bzero(micro_Message, sizeof(micro_Message));   // clear the micro_Message
        bzero(client_message, sizeof(client_message)); // clear the client_message
    }

    close(client_sock);
    close(socket_desc);

    return 0;
}

int main()
{
    indirect();
}
