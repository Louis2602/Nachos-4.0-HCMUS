#include "syscall.h"

int main()
{
    int status = -1, socketid = -1, noBytes = 0;
    char *ip = "127.0.0.1";
    int port = 9000;

    char buffer[250];
    int len = sizeof(buffer);
    char *data = "Hello World";

    int dataSize = 0;
    while (data[dataSize] != '\0')
        dataSize++;

    // Open a socket
    socketid = SocketTCP();
    // Connect to a server
    status = Connect(socketid, ip, port);
    // Send data to the server
    noBytes = Send(socketid, data, dataSize);
    // Receive buffer echoed back from server
    noBytes = Receive(socketid, data, len);
    Halt();
}
