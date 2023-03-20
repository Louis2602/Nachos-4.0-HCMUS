#include "syscall.h"

int main()
{
    int id = 1;
    int i;

    for (i = 0; i < 5; i++)
    {
        Open("file.txt", 0);
    }
    Close(4);
    Open("file.txt", 0);

    Halt();
}