#include "syscall.h"

int main()
{
    int status;
    char *ip = "127.0.0.1";
    int port = 80;
    int sockID = 6;
    status = Connect(sockID, ip, port);
    Halt();
}
