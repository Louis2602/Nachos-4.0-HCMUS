#include <arpa/inet.h>
#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_LENGTH 2000

/*
 * Convert message recv from client to uppercase
 * */

void convertToUppercase(char *givenStr)
{
    int i;
    for (i = 0; givenStr[i] != '\0'; i++)
    {
        if (givenStr[i] >= 'a' && givenStr[i] <= 'z')
        {
            givenStr[i] = givenStr[i] - 32;
        }
    }
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *sockfd)
{
    // Get the socket descriptor
    int sock = *(int *)sockfd;
    int rc;
    char client_message[BUFFER_LENGTH];

    // Receive a message from client
    while ((rc = recv(sock, client_message, sizeof(client_message), 0)) > 0)
    {
        // Receive data sent from client
        printf("%d bytes of data were received.\n", rc);
        printf("Data received from client: %s\n", client_message);

        // Send the message back to client
        convertToUppercase(client_message);
        printf("Data echoed back to client: %s\n", client_message);
        printf("Number of bytes sent: %ld.\n", strlen(client_message));
        rc = send(sock, client_message, strlen(client_message), 0);
    }

    if (rc == 0)
    {
        printf("Client disconnected\n");
        fflush(stdout);
    }
    else if (rc == -1)
    {
        printf("Error: Recv failed\n");
    }

    // Free the socket pointer
    free(sockfd);
}

int main(int argc, char const *argv[])
{
    char *ip = "127.0.0.1";
    int port = 9000;
    int rc;
    // define server address
    struct sockaddr_in servAddr, client;
    int sockfd = -1, clientSocket = -1;
    int *new_sock;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Error: Socket failed.\n");
        return -1;
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip,
              &servAddr.sin_addr); // Convert IP address to binary form

    // bind socket to the specified IP and port
    rc = bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (rc < 0)
    {
        printf("Error: Bind failed.\n");
        return -1;
    }
    // listen for connections
    rc = listen(sockfd, 20);
    if (rc < 0)
    {
        printf("Error: Listen failed.\n");
        return -1;
    }
    int addrlen = sizeof(servAddr);
    printf("Server runs at port: %d, ip: %s, sockid: %d\n", port, ip, sockfd);
    printf("Ready for client connect.\n");
    // integer to hold client socket.
    while ((clientSocket = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&addrlen)))
    {
        printf("Success: Client connect through socketid: %d.\n", clientSocket);
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = clientSocket;

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
        {
            printf("Error: Could not create thread\n");
            return -1;
        }
        if (clientSocket < 0)
        {
            printf("Error: Accept failed.\n");
            return -1;
        }
    }
    // char buffer[BUFFER_LENGTH];
    // // Receive data sent from client
    // rc = recv(clientSocket, buffer, sizeof(buffer), 0);
    // if (rc < 0)
    // {
    //     printf("Error: Receive failed.\n");
    //     return -1;
    // }
    // printf("%d bytes of data were received.\n", rc);
    // printf("Data received from client: %s\n", buffer);

    // if (rc == 0 || rc < sizeof(buffer))
    // {
    //     printf("The client closed the connection before all of the\n");
    //     printf("data was sent\n");
    // }

    // convertToUppercase(buffer);
    // printf("Data echoed back to client: %s\n", buffer);
    // printf("Size: %ld\n", strlen(buffer));
    // // Echo data back to client
    // rc = send(clientSocket, buffer, strlen(buffer), 0);
    // if (rc < 0)
    // {
    //     printf("Error: Send failed.\n");
    //     return -1;
    // }
    // // Close down any open socket descriptors
    // if (sockfd == -1)
    //     close(sockfd);
    // if (clientSocket == -1)
    //     close(clientSocket);
    return 0;
}
