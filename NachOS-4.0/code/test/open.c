#include "syscall.h"

int main()
{
    char fileName[] = "file1.txt";
    int length, id;
    int i;

    for (i = 0; i < 5; i++)
    {
        id = Open("hello1.txt", 0);
    }
    Remove("hello1.txt");
    Close(2);
    Close(3);
    Close(4);
    Close(5);
    Close(6);
    Remove("hello1.txt");
    Halt();
}