#include "syscall.h"

int main()
{
    int status;
    char *buffer;
    ReadString(buffer, 255);
    PrintString(buffer);
    status = Create(buffer);
    Halt();
}