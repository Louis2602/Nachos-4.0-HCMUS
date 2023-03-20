#include "syscall.h"

int main()
{
    int status;
    char *buffer;
    ReadString(buffer, 255);
    status = Create(buffer);
    PrintString(buffer);
    Halt();
}