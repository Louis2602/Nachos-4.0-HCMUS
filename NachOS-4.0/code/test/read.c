#include "syscall.h"

int main()
{
    char buffer[100];
    int i;
    int write, id;
    int read;

    id = Open("hello.txt", 0);
    read = Read(buffer, 500, id);
    // int len = 0;
    // while (buffer[len] != '\0') ++len;
    PrintString("Read characters: ");
    PrintString(buffer);
    PrintString("\n");
    Close(id);
    Halt();
}