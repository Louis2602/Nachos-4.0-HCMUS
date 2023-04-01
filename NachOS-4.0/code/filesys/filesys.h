// filesys.h
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system.
//	The "STUB" version just re-defines the Nachos file system
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.
//
//	The other version is a "real" file system, built on top of
//	a disk simulator.  The disk is simulated using the native UNIX
//	file system (in a file named "DISK").
//
//	In the "real" implementation, there are two key data structures used
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "sysdep.h"
#include "openfile.h"
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>

#define MAX_PROCESS 20
#define MODE_READ_AND_WRITE 0
#define MODE_READ 1
#define MODE_WRITE 2

typedef int OpenFileId;

struct FileSocket
{
  int type;
  int id;
};

// #define FILESYS_STUB
#ifdef FILESYS_STUB // Temporarily implement file system calls as
// calls to UNIX, until the real file system
// implementation is available
class FileSystem
{

public:
  OpenFile **fileTable;
  int *fileOpenType;
  char **fileName;
  int *fdTable;
  FileSocket fdFileSocket[MAX_PROCESS];
  int index;

  FileSystem()
  {
    fileTable = new OpenFile *[MAX_PROCESS];
    fileName = new char *[MAX_PROCESS];
    fdTable = new int[MAX_PROCESS];
    fileOpenType = new int[MAX_PROCESS];
    index = 0;
    for (int i = 0; i < MAX_PROCESS; i++)
    {
      fileTable[i] = NULL;
      fileName[i] = NULL;
      fdTable[i] = NULL;
      fdFileSocket[i].type = -1;
    }
    printf("Initial FD Table for sockets\n");
    for (int i = 0; i < 4; i++)
      printf("Available ID: %d\n", fdTable[i]);

    fileOpenType[0] = MODE_READ;
    fileOpenType[1] = MODE_WRITE;
  }

  ~FileSystem()
  {
    for (int i = 0; i < MAX_PROCESS; i++)
    {
      if (fileTable[i] != NULL)
        delete fileTable[i];
      if (fileName[i] != NULL)
        delete fileName[i];
    }
    delete[] fileTable;
    delete[] fileName;
    delete[] fdTable;
    for (int i = 0; i < MAX_PROCESS; i++)
      fdTable[i] = NULL;
    printf("Clear FD Table for next sockets\n");
    for (int i = 0; i < 4; i++)
      printf("Available ID: %d\n", fdTable[i]);
  }

  bool Create(char *name, int initialSize)
  {
    int fileDescriptor = OpenForWrite(name);

    if (fileDescriptor == -1)
      return FALSE;
    Close(fileDescriptor);

    return TRUE;
  }

  OpenFile *Open(char *name)
  {
    int fileDescriptor = OpenForReadWrite(name, FALSE);

    if (fileDescriptor == -1)
      return NULL;
    return new OpenFile(fileDescriptor);
  }

  // Overload lai ham Open de mo file voi 2 type khac nhau
  int Open(char *name, int type)
  {
    int freeIndex1 = -1, freeIndex2 = -1, fileDescriptor = -1;
    for (int i = 2; i < MAX_PROCESS; i++)
    { // 0 and 1 is for stdin and stdout
      if (fileTable[i] == NULL)
      {
        freeIndex1 = i;
        break;
      }
    }
    for (int i = 2; i < MAX_PROCESS; i++)
      if (fdFileSocket[i].type == -1)
      {
        freeIndex2 = i;
        break;
      }
    // printf("free index1: %d\n", freeIndex1);
    // printf("free index2: %d\n", freeIndex2);
    // printf("\nopen mode: %d", type);
    if (freeIndex2 == -1 || freeIndex1 == -1)
      return -1;

    if (type == MODE_READ_AND_WRITE)
      fileDescriptor = OpenForReadWrite(name, FALSE);
    if (type == MODE_READ)
      fileDescriptor = OpenForRead(name, FALSE);

    // printf("\nfile descriptor: %d", fileDescriptor);
    if (fileDescriptor == -1)
      return -1;

    fileTable[freeIndex1] = new OpenFile(fileDescriptor);
    fileName[freeIndex1] = new char[strlen(name) + 1];
    strcpy(fileName[freeIndex1], name);
    fileOpenType[freeIndex1] = type;
    // printf("fileName[freeIndex]: %s\n", fileName[freeIndex]);

    fdFileSocket[freeIndex2].type = 0;
    fdFileSocket[freeIndex2].id = freeIndex1;

    // printf("Position: %d\n", freeIndex);
    // printf("Type: %d\n", type);

    return freeIndex2;
  }

  int Close(int id)
  {
    // for (int i = 0; i < MAX_PROCESS; i++)
    //   printf("%d ", fdFileSocket[i].type);
    // printf("\n");
    // printf("id: %d\n", id);
    if (id >= 0 && id < MAX_PROCESS && fdFileSocket[id].type != -1)
    {
      // printf("fdFileSocket[id].type: %d\n", fdFileSocket[id].type);
      int id1 = fdFileSocket[id].id;
      delete fileTable[id1];
      fileTable[id1] = NULL;
      fdFileSocket[id].type = -1;
    }
    else
      return -1;
    return 0;
  }

  int Read(int id, char *buffer, int charCount)
  {
    if (id >= MAX_PROCESS)
      return -1;
    if (fdFileSocket[id].id == -1)
      return -1;
    int result;
    if (fdFileSocket[id].type == 0)
    {
      result = fileTable[fdFileSocket[id].id]->Read(buffer, charCount);
      if (result != charCount)
        return -2;
    }
    else
    {
      result = Receive(id, buffer, charCount);
      if (result > charCount)
        return -2;
    }
    // if we cannot read enough bytes, we should return -2
    return result;
  }

  int Write(int id, char *buffer, int charCount)
  {
    if (id >= MAX_PROCESS)
      return -1;
    if (fdFileSocket[id].id == -1)
      return -1;

    if (fdFileSocket[id].type == 0 && fileOpenType[fdFileSocket[id].id] == MODE_READ)
      return fileTable[fdFileSocket[id].id]->Write(buffer, charCount);
    return Send(id, buffer, charCount);
  }

  int Seek(int pos, int id)
  {
    if (id <= 1 || id >= MAX_PROCESS)
      return -1;
    if (fdFileSocket[id].type == -1)
      return -1;
    // use seek(-1) to move to the end of file
    if (pos == -1)
      pos = fileTable[fdFileSocket[id].id]->Length();
    if (pos < 0 || pos > fileTable[fdFileSocket[id].id]->Length())
      return -1;
    return fileTable[fdFileSocket[id].id]->Seek(pos);
  }

  int SocketTCP()
  {
    int freeIndex = -1, sockfd = -1;
    for (int i = 0; i < MAX_PROCESS; i++)
    {
      if (fdTable[i] == NULL)
      {
        freeIndex = i;
        break;
      }
      if (fdFileSocket[i].id == NULL)
      {
        freeIndex = i;
        break;
      }
    }
    if (freeIndex == -1)
      return -1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
      printf("Error: Socket failed.\n");
      return -1;
    }
    fdTable[freeIndex] = sockfd;
    fdFileSocket[freeIndex].type = 1;
    fdFileSocket[freeIndex].id = sockfd;

    printf("FREE INDEX: %d\n", freeIndex);
    for (int i = 0; i < 4; i++)
    {
      printf("Available ID: %d\n", fdTable[i]);
      printf("Type: %d\n", fdFileSocket[i]);
      printf("ID: %d\n", fdFileSocket[i]);
    }

    return sockfd;
  }
  int CloseSocketTCP(int socketid)
  {
    int rc = close(socketid);
    if (rc < 0)
    {
      printf("Error: Fail to close a socket\n");
      return -1;
    }
    return 0;
  }
  int Connect(int socketid, char *ip, int port)
  {
    printf("======= Start connecting to server =======\n");
    printf("SocketID: %d\n", socketid);
    printf("Ip: %s\n", ip);
    printf("Port: %d\n", port);

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr)); // Clear the server address structure

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servAddr.sin_addr); // Convert IP address to binary form
    int rc = connect(socketid, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (rc < 0)
    {
      printf("Error: Fail to connect to a socket\n");
      return -1;
    }
    return 0;
  }
  int Send(int socketid, char *buffer, int len)
  {
    printf("======= Start sending data to server =======\n");
    printf("Data sent: %s\n", buffer);
    printf("Size: %d\n", len);

    int rc = send(socketid, buffer, len, 0);
    if (rc < 0)
    {
      printf("Error: Send data to server failed\n");
      return -1;
    }
    printf("Success: Data sent successfully.\n");
    return rc;
  }
  int Receive(int socketid, char *buffer, int len)
  {
    printf("======= Start receiving data from server =======\n");
    // int bytesReceived = 0;
    // while (bytesReceived < len)
    // {
    //   int rc = recv(socketid, &buffer[bytesReceived], len - bytesReceived, 0);
    //   printf("Bytes receive: %d\n", rc);
    //   if (rc < 0)
    //   {
    //     printf("Error: Receive data from server failed\n");
    //     return -1;
    //   }
    //   else if (rc == 0)
    //     printf("The server closed the connection\n");
    //   bytesReceived += rc;
    // }
    int rc = recv(socketid, buffer, len, 0);
    if (rc < 0)
    {
      printf("Error: Receive data from server failed\n");
      return -1;
    }
    printf("Success: Data received successfully.\n");
    return rc;
  }
  bool Remove(char *name)
  {
    for (int id = 2; id < MAX_PROCESS; id++)
      if (fileTable[id] != NULL && strcmp(fileName[id], name) == 0)
      {
        printf("You must close `%s` before deleting. Please try again!\n", fileName[id]);
        return 0;
      }

    return Unlink(name) == 0;
  }
};

#else // FILESYS
class FileSystem
{
public:
  OpenFile **fileTable;
  int index;

  FileSystem(bool format); // Initialize the file system.
                           // Must be called *after* "synchDisk"
                           // has been initialized.
                           // If "format", there is nothing on
                           // the disk, so initialize the directory
                           // and the bitmap of free blocks.

  bool Create(char *name, int initialSize);
  // Create a file (UNIX creat)

  OpenFile *Open(char *name); // Open a file (UNIX open)
  // OpenFile *Open(char *name, int type);

  int Close(OpenFileId id);

  bool Remove(char *name); // Delete a file (UNIX unlink)

  void List(); // List all the files in the file system

  void Print(); // List all the files and their contents

  int Read(int id, char *buffer, int charCount);
  int Write(int id, char *buffer, int charCount);
  int Seek(int pos, int id);
  int Open(char *name, int type);

  int SocketTCP();
  int Connect(int socketid, char *ip, int port);
  int Send(int socketid, char *buffer, int len);
  int Receive(int socketid, char *buffer, int len);
  int CloseSocketTCP(int socketid);

private:
  OpenFile *freeMapFile;   // Bit map of free disk blocks,
                           // represented as a file
  OpenFile *directoryFile; // "Root" directory -- list of
                           // file names, represented as a file
};

#endif // FILESYS

#endif // FS_H
