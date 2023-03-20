#include "syscall.h"

int main()
{
    char buffer[500];
    char bufferAfter[500];
    char *buffer4Write;
    int i;
    int write, id, len;
    int read;

    id = Open("hello.txt", 0);
    read = Read(buffer, 500, id);
    // int len = 0;
    // while (buffer[len] != '\0') ++len;
    PrintString("Before: ");
    PrintString(buffer);
    PrintString("\n");

    PrintString("Write something new: ");
    ReadString(buffer4Write, 255);
    len = 0;
    while (buffer4Write[len] != '\0') ++len;

    Write(buffer4Write, len, id);
    
    Close(id);
    
    id = Open("hello.txt", 0);
    read = Read(bufferAfter, 500, id);
    PrintString("After: ");
    PrintString(bufferAfter);
    PrintString("\n");

    Halt();
}