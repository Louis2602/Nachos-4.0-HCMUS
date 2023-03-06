/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

bool SysCreate(char *fileName)
{
  bool success;
  int fileNameLength = strlen(fileName);

  if (fileNameLength == 0)
  {
    DEBUG(dbgSys, "\nFile name can't be empty");
    success = false;
  }
  else if (fileName == NULL)
  {
    DEBUG(dbgSys, "\nNot enough memory in system");
    success = false;
  }
  else
  {
    DEBUG(dbgSys, "\nFile's name read successfully");
    if (!kernel->fileSystem->Create(fileName, 0))
    {
      DEBUG(dbgSys, "\nError creating file");
      success = false;
    }
    else
    {
      success = true;
    }
  }

  return success;
}
int SysOpen(char *fileName, int type)
{
  if (type != 0 && type != 1)
    return -1;
  // Open file successfully
  if (kernel->fileSystem->Open(fileName))
  {
    DEBUG(dbgSys, "\nOpened file");
  }
  return 1;
}

int SysClose(int id)
{
  return kernel->fileSystem->Close(id);
}
int SysSocketTCP()
{
  return kernel->fileSystem->SocketTCP();
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
