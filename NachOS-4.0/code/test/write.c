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
    read = Read(id, buffer, 500);
    // int len = 0;
    // while (buffer[len] != '\0') ++len;
    PrintString("Before: ");
    PrintString(buffer);
    PrintString("\n");

    PrintString("Write something new: ");
    ReadString(buffer4Write, 255);
    len = 0;
    while (buffer4Write[len] != '\0')
        ++len;

    Write(id, buffer4Write, len);

    Close(id);

    id = Open("hello.txt", 0);
    read = Read(id, bufferAfter, 500);
    PrintString("After: ");
    PrintString(bufferAfter);
    PrintString("\n");

    Halt();
}