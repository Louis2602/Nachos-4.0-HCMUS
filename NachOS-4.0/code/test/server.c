#include <arpa/inet.h>
#include <netinet/in.h> //structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_LENGTH 250

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

int main(int argc, char const *argv[])
{
    char *ip = "127.0.0.1";
    int port = 9000;
    int rc;
    // define server address
    struct sockaddr_in servAddr;
    int sockfd = -1, clientSocket = -1;

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

    printf("Server runs at port: %d, ip: %s, sockid: %d\n", port, ip, sockfd);
    printf("Ready for client connect.\n");
    // integer to hold client socket.
    clientSocket = accept(sockfd, NULL, NULL);
    printf("Success: Client connect through socketid: %d.\n", clientSocket);
    if (clientSocket < 0)
    {
        printf("Error: Accept failed.\n");
        return -1;
    }
    char buffer[BUFFER_LENGTH];
    // Receive data sent from client
    rc = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (rc < 0)
    {
        printf("Error: Receive failed.\n");
        return -1;
    }
    printf("%d bytes of data were received.\n", rc);
    printf("Data received from client: %s\n", buffer);

    if (rc == 0 || rc < sizeof(buffer))
    {
        printf("The client closed the connection before all of the\n");
        printf("data was sent\n");
    }

    convertToUppercase(buffer);
    printf("Data echoed back to client: %s\n", buffer);
    printf("Size: %ld\n", strlen(buffer));
    // Echo data back to client
    rc = send(clientSocket, buffer, strlen(buffer), 0);
    if (rc < 0)
    {
        printf("Error: Send failed.\n");
        return -1;
    }
    // Close down any open socket descriptors
    if (sockfd == -1)
        close(sockfd);
    if (clientSocket == -1)
        close(clientSocket);
    return 0;
}
