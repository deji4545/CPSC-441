//Name - Ayodeji Osho
//Class - CPSC 441 T07
//Student ID -30071771

//REFERENCE - https://stackoverflow.com/questions/9935642/how-do-i-use-strcasestr
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <stdlib.h>
#include <ctype.h>

#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>

//Reference - TA Code From exampleClient and exampleServer was used
//Create the socket for client side of socket
int proxyClient(char *server_reply, char *request, int client_sock)
{
    int sock;
    struct sockaddr_in server;
    //char server_reply[2000];

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //extracting host name
    //Reference - https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/
    char hostbuffer[256] = "pages.cpsc.ucalgary.ca";
    struct hostent *host_entry;
    int hostname;

    // To retrieve host information
    host_entry = gethostbyname(hostbuffer);

    // To convert an Internet network address into ASCII string
    server.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0])));
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        //return 1;
    }

    puts("Connected\n");

    //Send some data
    if (send(sock, request, strlen(request), 0) < 0)
    {
        puts("Send failed");
    }

    char buffer[4096];
    int buff_size = 0;
    int x = 0;

    //Receive a reply from the server
    while (1)
    {
        buff_size = recv(sock, buffer, sizeof(buffer), 0); //receive the jpeg file

        send(client_sock, buffer, buff_size, 0); //send to browser

        if (buff_size <= 0)
        {
            break;
        }
    }

    close(sock);
    return 0;
}

//Create socket for blocked request to access
int blocked()
{

    int socket_desc;
    struct sockaddr_in server;
    //Set a time interval for connection
    //Reference - https://stackoverflow.com/questions/2876024/linux-is-there-a-read-or-recv-from-socket-with-timeout
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
    }
    puts("Socket created\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1568);

    //Reference - https://stackoverflow.com/questions/24194961/how-do-i-use-setsockoptso-reuseaddr
    //Allows port to be reused again
    int enable = 1;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
    {
        printf("setsockopt(SO_REUSEADDR) failed");
    }

    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);

    //Bind
    int bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));

    if (bindStatus == -1)
    {
        perror("Binding failed\n");
    }

    printf("You have 10 seconds to connect. If you fail to. Try again\n");
    listen(socket_desc, 3);

    return socket_desc;
}

//Create the server side of proxy
int proxyServer()
{

    int socket_desc, client_sock, block_socket_desc, block_client_sock;
    struct sockaddr_in server, client;
    char client_message[5000];
    int recvStatus;
    int block_status;
    char server_reply[2000];

    //Set a time interval for connection
    //Reference - https://stackoverflow.com/questions/2876024/linux-is-there-a-read-or-recv-from-socket-with-timeout
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    int max_strings = 15;
    int max_char = 25;

    char keywords_list[max_strings][max_char];
    char blocked_message[] = "GET http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html HTTP/1.1\r\nHost: https://pages.cpsc.ucalgary.ca\r\nConnection: close\r\nAccept-language: en\r\n\r\n";

    int i;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
        return -1;
    }
    //puts("Socket created\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1027);

    //Reference - https://stackoverflow.com/questions/24194961/how-do-i-use-setsockoptso-reuseaddr
    //Allows port to be reused again
    int enable = 1;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
    {
        printf("setsockopt(SO_REUSEADDR) failed");
    }
    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);

    //Bind
    int bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));

    if (bindStatus == -1)
    {
        perror("Binding failed For proxy server");
    }
    else
    {
        printf("Binding done for proxy server.\n");
    }

    //create socket for blocked request
    block_socket_desc = blocked();

    block_client_sock = accept(block_socket_desc, NULL, NULL);

    int x = 0;

    while (1)
    {
        block_client_sock = accept(block_socket_desc, NULL, NULL);

        if (x > 5 && (block_client_sock == -1))
        {
            return -1;
        }
        else if (block_client_sock != -1)
        {
            printf("You have sucessfully connected\n");
            break;
        }

        x = x + 1;
    }

    //Listen
    listen(socket_desc, 3);

    // setsockopt(block_socket_desc, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tvb, sizeof tvb);
    int count = 0;
    int max_count = 0;

    for (i = 0; i < max_strings; i++)
    {
        strcpy(keywords_list[i], " ");
    }

    while (1)
    {
        for (i = 0; i < max_strings; i++)
        {
            if (strcmp(keywords_list[i], " ") == 0)
            {
                count = i;
                if (count > max_count)
                {
                    max_count = count;
                }
                break;
            }
        }

        block_status = recv(block_client_sock, keywords_list[count], 1000, 0);

        if (block_status != -1)
        {

            for (size_t i = 0; i < strlen(keywords_list[count]); ++i)
            {
                keywords_list[count][i] = tolower((unsigned char)keywords_list[count][i]);
            }

            char *keyword = keywords_list[count];

            char *ptr = strchr(keyword, '\r');
            strcpy(ptr, "");

            if (strstr(keyword, "unblock") != NULL)
            {

                keyword = keyword + 8;
                strcpy(keywords_list[count], keyword);
                printf("Unblocked keyword is %s\n", keywords_list[count]);
                for (i = 0; i < max_count; i++)
                {

                    if (i != count)
                    {
                        if (strcmp(keywords_list[i], keyword) == 0)
                        {
                            strcpy(keywords_list[i], " ");
                        }
                    }
                }
                strcpy(keywords_list[i], " ");
            }
            else if (strstr(keyword, "block") != NULL)
            {
                keyword = keyword + 6;

                strcpy(keywords_list[count], keyword);
                printf("Blocked keyword is %s\n", keywords_list[count]);
            }
            else
            {
                printf("Invalid Input\n");
            }
        }
        client_sock = accept(socket_desc, NULL, NULL);
        int someBool = 1;

        //Receive a message from client
        recvStatus = recv(client_sock, client_message, 5000, 0);

        if (recvStatus != -1)
        {
            char *ucalgary = strstr(client_message, "ucalgary");

            if (ucalgary != NULL)
            {

                // Loop through keywords blocked list and if it is part of the http request, the website is blocked
                for (i = 0; i < max_count; i++)
                {

                    if (strcmp(keywords_list[i], " ") != 0)
                    {
                        if (strcasestr(client_message, keywords_list[i]) != NULL)
                        {
                            someBool = 0;
                            break;
                        }
                    }
                }

                //Sends the actual client request or a blocked request based on keywords in blocked list
                if (someBool == 1)
                {
                    proxyClient(server_reply, client_message, client_sock);
                    printf("%s", client_message);
                }
                else
                {
                    proxyClient(server_reply, blocked_message, client_sock);
                    printf("The client request has been blocked\n");
                    printf("%s", client_message);
                }
            }
        }
    }

    //close all socket
    close(socket_desc);
    close(client_sock);
    close(block_socket_desc);
    close(block_client_sock);
    return 0;
}

int main()
{
    proxyServer();
    return 0;
}