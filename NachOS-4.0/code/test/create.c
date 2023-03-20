#include "syscall.h"

int main()
{
    int status;
    char *buffer;
    status = Create("newFile.txt");
    ReadString(buffer, 255);
    PrintString(buffer);
    Halt();
}