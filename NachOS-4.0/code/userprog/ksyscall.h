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
      printf("Erroe creating file `%s`.\n", fileName);
      success = false;
    } else {
      DEBUG(dbgSys, "Creating a file successfully.\n");
      printf("Creating a file `%s` successfully.\n", fileName);
      success = true;
    }
  }

  return success;
}
int SysOpen(char *fileName, int type) {
  // if (type != 0 && type != 1)
  //   return -1;
  // // Open file successfully
  // if (kernel->fileSystem->Open(fileName))
  // {
  //   DEBUG(dbgSys, "\nOpened file");
  // }
  return 1;
}

int SysClose(int id) {
  // return kernel->fileSystem->Close(id);
  return 1;
}
int SysSocketTCP() {
  int sockID = kernel->fileSystem->SocketTCP();
  printf("Socket ID: `%d`.\n", sockID);
  return sockID;
}

int SysConnect(int sockID, char *ip, int port) {
  int new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  int opt = 1;
  // Creating socket file descriptor
  if ((sockID = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  // Forcefully attaching socket to the port 8080
  if (setsockopt(sockID, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // Forcefully attaching socket to the port 8080
  if (bind(sockID, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(sockID, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  if ((new_socket = accept(sockID, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  if (connect(sockID, (struct sockaddr *)&sockaddr_in, sizeof(sockaddr_in)) ==
      -1) {
    perror("connect");
    return EXIT_FAILURE;
  }
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
