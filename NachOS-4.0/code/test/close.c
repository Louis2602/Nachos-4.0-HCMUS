#include "syscall.h"

int main()
{
    int id = 1;
    int i;

    for (i = 0; i < 5; i++)
    {
        id = Open("file.txt", 0);
    }
    Close(id);
    Open("file.txt", 0);

    Halt();
}