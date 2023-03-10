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

#ifdef FILESYS_STUB // Temporarily implement file system calls as
// calls to UNIX, until the real file system
// implementation is available
class FileSystem
{
public:
  FileSystem() {}

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

  int SocketTCP()
  {
    int sockID = OpenSocket();
    return sockID;
  }
  int Connect(int socketid, char *ip, int port)
  {
    // // Open a socket
    // printf("Connect to socket with ID: `%d`\n", socketid);
    // AssignNameToSocket("Testing socket", socketid); // Bind socket to a filename
    // // Creating socket file descriptor
    // if (socketid == -1)
    // {
    //   printf("Error: Could not create socket\n");
    //   return -1;
    // }
    // struct sockaddr_in server;

    // server.sin_family = AF_INET;
    // server.sin_addr.s_addr = inet_addr(ip);
    // // server.sin_addr.s_addr = INADDR_ANY;
    // server.sin_port = htons(port);

    // // Connect to remote server
    // if (connect(socketid, (struct sockaddr *)&server, sizeof(server)) < 0)
    // {
    //   printf("Error: Fail to connect to remote server\n");
    //   return -1;
    // }
    // else
    // {
    //   printf("Success: Connected to remote server\n");
    //   char strData[255];
    //   recv(socketid, strData, sizeof(strData), 0);
    //   printf("Message: %s\n", strData);
    // }
    // return 0;
    int sockD = socket(AF_INET, SOCK_STREAM, 0);
    printf("SocketID: %d\n", sockD);

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9001); // use some unused port number
    servAddr.sin_addr.s_addr = INADDR_ANY;

    int connectStatus = connect(sockD, (struct sockaddr *)&servAddr,
                                sizeof(servAddr));

    if (connectStatus == -1)
    {
      printf("Error...\n");
    }

    else
    {
      char strData[255];

      recv(sockD, strData, sizeof(strData), 0);

      printf("Message: %s\n", strData);
    }

    return 0;
  }
  bool Remove(char *name) { return Unlink(name) == 0; }
};

#else // FILESYS
class FileSystem
{
public:
  FileSystem(bool format); // Initialize the file system.
                           // Must be called *after* "synchDisk"
                           // has been initialized.
                           // If "format", there is nothing on
                           // the disk, so initialize the directory
                           // and the bitmap of free blocks.

  bool Create(char *name, int initialSize);
  // Create a file (UNIX creat)

  OpenFile *Open(char *name); // Open a file (UNIX open)

  bool Remove(char *name); // Delete a file (UNIX unlink)

  void List(); // List all the files in the file system

  void Print(); // List all the files and their contents

  int Close(int id)
  {
    return 1;
    // Implement in here
  }
  int SocketTCP();
  int Connect(int socketid, char *ip, int port);
  int Send(int socketid, char *buffer, int len);
  int Receive(int socketid, char *buffer, int len);
  int Close(int socketid);

private:
  OpenFile *freeMapFile;   // Bit map of free disk blocks,
                           // represented as a file
  OpenFile *directoryFile; // "Root" directory -- list of
                           // file names, represented as a file
};

#endif // FILESYS

#endif // FS_H
