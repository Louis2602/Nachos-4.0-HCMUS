#include "syscall.h"

int main()
{
    char buffer[100];
    int fileId;
    int read;

    fileId = Open("hello.txt", 0);
    if (fileId != -1)
    {
        read = Read(buffer, 500, fileId);
        PrintString("Read characters: \n");
        PrintString(buffer);
        PrintString("\n");
        Close(fileId);
    }
    else
    {
        PrintString("Loi khi mo src file");
    }
    Halt();
}