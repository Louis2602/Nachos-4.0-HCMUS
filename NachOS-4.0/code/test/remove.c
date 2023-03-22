#include "syscall.h"

int main(int argc, char const * argv[])
{
  int status = Remove("hello.txt");
  Halt();
}