#include "syscall.h"

int main()
{
    int status = Create("hello.txt");
    Halt();
}