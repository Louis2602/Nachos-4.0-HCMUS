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

#ifdef FILESYS_STUB // Temporarily implement file system calls as
// calls to UNIX, until the real file system
// implementation is available
class FileSystem
{
public:
  FileSystem() {}

  bool Create(char *name, int initialSize)
  {
    if (strcmp(name, "newFile.txt") != 0)
    {
      int fileDescriptor = OpenForWrite(name);

      if (fileDescriptor == -1)
        return FALSE;
      Close(fileDescriptor);
    }
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
    int MAX_FDS = 20;
    int fd_table[MAX_FDS];
    // Index of the next available file descriptor
    int sockfd, next_fd = 0;
    // Check if we have reached the maximum number of file descriptors
    while (next_fd < MAX_FDS)
    {
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd == -1)
      {
        printf("Error: Socket failed.\n");
        return -1;
      }
      fd_table[next_fd] = sockfd;
      next_fd++;
    }
    for (int i = 0; i < MAX_FDS; ++i)
      printf("Socket number %d has ID: `%d`\n", i + 1, fd_table[i]);

    return sockfd;
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

private:
  OpenFile *freeMapFile;   // Bit map of free disk blocks,
                           // represented as a file
  OpenFile *directoryFile; // "Root" directory -- list of
                           // file names, represented as a file
};

#endif // FILESYS

#endif // FS_H
