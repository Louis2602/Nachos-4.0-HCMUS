#include "syscall.h"

#define BUFFER_LENGTH 255

int main()
{
  int status = -1, socketid = -1, noBytes = 0;
  char *ip = "127.0.0.1";
  int port = 9000;

  // char *data = "Hello World";
  char buffer[100];
  int srcFileId, destFileId;
  int read, write;
  int len = 0;

  // Open a socket
  socketid = SocketTCP();
  // Connect to a server
  status = Connect(socketid, ip, port);

  srcFileId = Open("a.txt", 0);
  if (srcFileId != -1)
  {
    destFileId = Create("b.txt");
    destFileId = Open("b.txt", 0);

    if (destFileId != -1) // Kiem tra mo file thanh cong
    {
      read = Read(srcFileId, buffer, 500);
      while (buffer[len] != '\0')
        ++len;

      // Send data to the server
      noBytes = Send(socketid, buffer, len);

      // Receive buffer echoed back from server
      noBytes = Receive(socketid, buffer, len);

      len = 0;
      while (buffer[len] != '\0')
        ++len;

      Write(destFileId, buffer, len);
      Close(destFileId); // Goi ham Close de dong file dich
    }
    else
    {
      PrintString(" -> Tao dest file khong thanh cong!!!\n\n");
    }
    Close(srcFileId); // Goi ham Close de dong file nguon
  }
  else
  {
    PrintString("Loi khi mo src file");
  }

  // Close socket
  if (socketid == -1)
    status = CloseSocketTCP(socketid);

  Halt();
}
