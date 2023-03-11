#include "syscall.h"

int main()
{
    int status;
    int socketid = -1;
    char *ip = "127.0.0.1";
    int port = 9000;

    char buffer[250];
    int len = sizeof(buffer);
    char *data = "Hello from client";

    // Open a socket
    socketid = SocketTCP();
    // Connect to a server
    status = Connect(socketid, ip, port);
    // Send 250 bytes of a's to the server
    status = Send(socketid, data, len);
    // Receive buffer echoed back from server
    status = Receive(socketid, buffer, len);

    Halt();
}
