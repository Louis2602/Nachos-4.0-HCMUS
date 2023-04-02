#include "syscall.h"

int main()
{
    int status;
    char *buffer;
    PrintString("Input filename: ");
    ReadString(buffer, 255);
    status = Create(buffer);
    Halt();
}