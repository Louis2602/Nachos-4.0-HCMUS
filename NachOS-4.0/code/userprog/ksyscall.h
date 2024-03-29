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

#include "main.h"

void SysHalt() { kernel->interrupt->Halt(); }

int SysAdd(int op1, int op2) { return op1 + op2; }

bool SysCreate(char *fileName) {
  bool success;
  int fileNameLength = strlen(fileName);

  if (fileNameLength == 0) {
    DEBUG(dbgSys, "\nFile name can't be empty");
    success = false;
  } else if (fileName == NULL) {
    DEBUG(dbgSys, "\nNot enough memory in system");
    success = false;
  } else {
    DEBUG(dbgSys, "\nFile's name read successfully");
    if (!kernel->fileSystem->Create(fileName, 0)) {
      DEBUG(dbgSys, "\nError creating file");
      printf("Error: creating file `%s`.\n", fileName);
      success = false;
    }
    else
    {
      DEBUG(dbgSys, "Creating a file successfully.\n");
      printf("Success: Creating a file `%s` successfully.\n", fileName);
      success = true;
    }
  }

  return success;
}
int SysOpen(char *fileName, int type)
{
  // if (type != 0 && type != 1)
  //   return -1;
  // // Open file successfully
  // if (kernel->fileSystem->Open(fileName))
  // {
  //   DEBUG(dbgSys, "\nOpened file");
  // }
  return 1;
}

int SysClose(int id)
{
  // return kernel->fileSystem->Close(id);
  return 1;
}
int SysSocketTCP()
{
  int sockID = kernel->fileSystem->SocketTCP();
  printf("Socket ID: `%d`.\n", sockID);
  return sockID;
}

int SysConnect(int socketid, char *ip, int port)
{
  // char *ip = "127.0.0.1";
  // int port = 80;
  // int socketid = OpenSocket();

  int success = kernel->fileSystem->Connect(socketid, ip, port);
  printf("Success: %d\n", success);
  return success;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
