// #include <netinet/in.h> //structure for storing address information
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/socket.h> //for socket APIs
// #include <sys/types.h>
// #include <arpa/inet.h>
// #include <sys/un.h>
// #include <string.h>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERVER_PATH "/tmp/server"

int main(int argc, char const *argv[])
{

    int rc;
    // define server address
    struct sockaddr_un servAddr;
    int servSockD = -1, clientSocket = -1;
    // string store data to send to client
    char serMsg[255] = "Message from the server to the "
                       "client \'Hello Client\' ";
    do
    {
        servSockD = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (servSockD < 0)
        {
            printf("Error: Socket failed.\n");
            return -1;
        }

        // struct sockaddr_in servAddr;

        memset(&servAddr, 0, sizeof(servAddr));
        servAddr.sun_family = AF_UNIX;
        strcpy(servAddr.sun_path, SERVER_PATH);

        // inet_pton(AF_INET, ip, &servAddr.sin_addr); // Convert IP address to binary form

        // bind socket to the specified IP and port
        // bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr));
        rc = bind(servSockD, (struct sockaddr *)&servAddr, sizeof(sa_family_t));
        // rc = bind(servSockD, (struct sockaddr *)&servAddr, SUN_LEN(&servAddr));
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
        // send's messages to client socket
        rc = send(clientSocket, serMsg, sizeof(serMsg), 0);
        if (rc < 0)
        {
            printf("Error: Send failed.\n");
            return -1;
        }

    } while (0);

    return 0;
}
