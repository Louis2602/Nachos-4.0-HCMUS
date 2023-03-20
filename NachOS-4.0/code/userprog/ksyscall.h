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
  if (type != 0 && type != 1)
    return -1;
  // Open file successfully
  if (kernel->fileSystem->Open(fileName, type) != -1)
  {
    DEBUG(dbgSys, "\nOpened file");
    printf("Success: Opening a file `%s` successfully.\n", fileName);
  }
  else
  {
    DEBUG(dbgSys, "\nOpened file");
    printf("Failed: Opening a file `%s` failed.\n", fileName);
  }
  return 1;
}

int SysClose(int id)
{
  if (kernel->fileSystem->Close(id) != -1)
  {

    DEBUG(dbgSys, "\nClose file");
    printf("Success: File no.`%d` has been closed successfully.\n", id);
    return 1;
  }
  else
  {
    DEBUG(dbgSys, "\nClose file");
    printf("Success: Closing the file no.`%d` failed.\n", id);
  }
  return -1;
}
int SysSocketTCP()
{
  int sockID = kernel->fileSystem->SocketTCP();
  printf("Socket ID: `%d`.\n", sockID);
  return sockID;
}

int SysConnect(int socketid, char *ip, int port)
{
  int success = kernel->fileSystem->Connect(socketid, ip, port);
  printf("Success: %d\n", success);
  return success;
}

int SysSend(int socketid, char *buffer, int len)
{
  int noBytes = kernel->fileSystem->Send(socketid, buffer, len);
  printf("Number of bytes sent: %d\n", noBytes);
  return noBytes;
}

int SysReceive(int socketid, char *buffer, int len)
{
  int noBytes = kernel->fileSystem->Receive(socketid, buffer, len);
  printf("Number of bytes receive: %d\n", noBytes);
  printf("Data received from server: %s\n", buffer);
  return noBytes;
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
