#include "syscall.h"

int main()
{
    char fileName[] = "file.txt";
    int length, id;
    int i;

    for (i = 0; i < 25; i++)
    {
        id = Open(fileName, 0);
    }
    Halt();
}