#include "syscall.h"

int main()
{
    int status;
    status = Create("newFile.txt");
    Halt();
}