#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

// #define BUFFER_LENGTH 250

int main(int argc, char const *argv[])
{

    int rc;
    // define server address
    // struct sockaddr_un servAddr;
    struct sockaddr_in servAddr;
    int servSockD = -1, clientSocket = -1;
    // string store data to send to client
    char serMsg[255] = "Message from the server to the "
                       "client \'Hello Client\' ";
    servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD < 0)
    {
        printf("Error: Socket failed.\n");
        return -1;
    }

    memset(&servAddr, 0, sizeof(servAddr));
    char *ip = "127.0.0.1";
    int port = 9001;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    // servAddr.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, ip, &servAddr.sin_addr); // Convert IP address to binary form

    // bind socket to the specified IP and port
    rc = bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (rc < 0)
    {
        printf("Error: Bind failed.\n");
        return -1;
    }
    // listen for connections
    rc = listen(servSockD, 20);
    if (rc < 0)
    {
        printf("Error: Listen failed.\n");
        return -1;
    }

    printf("Ready for client connect.\n");
    // integer to hold client socket.
    clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket < 0)
    {
        printf("Error: Accept failed.\n");
        return -1;
    }
    // send messages to client socket
    rc = send(clientSocket, serMsg, sizeof(serMsg), 0);
    // send data to client socket
    // char buffer[BUFFER_LENGTH];
    // rc = send(clientSocket, buffer, sizeof(buffer), 0);
    if (rc < 0)
    {
        printf("Error: Send failed.\n");
        return -1;
    }
    return 0;
}
