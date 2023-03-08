#include "syscall.h"

int main()
{
    int sockID;
    sockID = SocketTCP();
    Halt();
}
