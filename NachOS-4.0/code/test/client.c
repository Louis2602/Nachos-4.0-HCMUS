#include "syscall.h"

#define BUFFER_LENGTH 255

int main()
{
    int status = -1, socketid = -1, noBytes = 0;
    char *ip = "127.0.0.1";
    int port = 9000;
    int dataSize = 0;

    char *data = "Hello World";
    // char *data;
    char buffer[BUFFER_LENGTH];
    int len = sizeof(buffer);
    int i;

    for (i = 0; i < 4; i++)
    {
        // Open a socket
        socketid = SocketTCP();
        // Connect to a server
        status = Connect(socketid, ip, port);

        // while (1)
        // {
        // PrintString("Enter message: ");
        // ReadString(data, BUFFER_LENGTH);
        while (data[dataSize] != '\0')
            dataSize++;
        // Send data to the server
        noBytes = Send(socketid, data, dataSize);
        // Receive buffer echoed back from server
        noBytes = Receive(socketid, data, len);
        // }
        // Close socket
        if (socketid == -1)
            status = CloseSocketTCP(socketid);
    }

    Halt();
}
