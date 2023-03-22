#include "syscall.h"

int main()
{
  char buffer[100];
  int srcFileId, destFileId;
  int read, write;
  int len = 0;

  srcFileId = Open("a.txt", 0);
  if (srcFileId != -1)
  {
    destFileId = Create("b.txt");
    destFileId = Open("b.txt", 0);

    if (destFileId != -1) // Kiem tra mo file thanh cong
    {
      read = Read(buffer, 500, srcFileId);
      while (buffer[len] != '\0')
        ++len;

      Write(buffer, len, destFileId);
      PrintString(" -> Copy thanh cong.\n\n");
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

  Halt();
}