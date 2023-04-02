#include "syscall.h"

int main(int argc, char const *argv[])
{
  int status;
  char *buffer;
  PrintString("Input filename: ");
  ReadString(buffer, 255);
  status = Remove(buffer);
  Halt();
}