#include "syscall.h"

int main()
{
    char fileName[] = "file.txt";
    int length, id;
    int i;

    for (i = 0; i < 5; i++)
    {
        id = Open("hello.txt", 0);
    }
    Halt();
}