// Name - Ayodeji Osho
// Class - CPSC 441 T07
// Student ID -30071771

#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[10], server_reply[2000], userInput[30];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("136.159.5.25");
    server.sin_family = AF_INET;
    server.sin_port = htons(1488);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");
    int j;

    // keep communicating with server
    while (1)
    {
        printf("\n");
        printf("Enter 1 for Translator Service\nEnter 2 for Currency Service\nEnter 3 for Voting\nEnter a value: ");
        fgets(message, 10, stdin);
        printf("\n");

        // Send some data
        send(sock, message, strlen(message), 0);
        int service_Num = atoi(message);

        if (service_Num == 1)
        {
            printf("What word do you wish to translate:\n");
        }
        else if (service_Num == 2)
        {
            printf("Enter: <Value> <CAD> <USD>\n");
        }
        if (service_Num == 3)
        {
            printf("Enter the candidate ID:");
        }

        // send and receive message based on user input for service number
        if (service_Num == 1 || service_Num == 2)
        {
            fgets(userInput, sizeof(userInput), stdin);
            send(sock, userInput, strlen(userInput), 0);
        }

        if (service_Num == 4)
        {
            send(sock, "4", 1, 0);
        }

        // Receive a reply from the server
        char encryptedMessage[10];

        if (service_Num == 3)
        {
            fgets(userInput, sizeof(userInput), stdin);
            recv(sock, server_reply, sizeof(server_reply), 0);
            printf("key: %s\n", server_reply);

            int encrypted_Message_Num = atoi(server_reply) * atoi(userInput);
            snprintf(encryptedMessage, sizeof(encryptedMessage), "%d", encrypted_Message_Num);
            printf("Encrypted Message: %s\n\n", encryptedMessage);

            // send encrypted message
            send(sock, encryptedMessage, strlen(encryptedMessage), 0);
            bzero(server_reply, sizeof(server_reply));
            printf("Name     ID     Votes\n");
        }

        recv(sock, server_reply, sizeof(server_reply), 0);

        printf("%s\n", server_reply);

        bzero(userInput, sizeof(userInput));
        bzero(server_reply, sizeof(server_reply));
    }

    close(sock);
    return 0;
}
