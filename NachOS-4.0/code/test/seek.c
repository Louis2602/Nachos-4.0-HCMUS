#include "syscall.h"

#define MODE_READ_AND_WRITE 0
#define MODE_READ 1
#define stdin 0
#define stdout 1

int main()
{
    int openFileID, fileSize_1, fileSize_2, file_1, file_2, file_3, i;
    char c;
    file_3 = Open("file3.txt", MODE_READ_AND_WRITE);
    if (file_3 != -1)
    {
        file_1 = Open("file1.txt", MODE_READ);
        if (file_1 != -1)
        {
            fileSize_1 = Seek(-1, file_1);
            PrintString("ok");
            Seek(0, file_1);
            Seek(0, file_3);
            i = 0;
            for (; i < fileSize_1; i++)
            {
                Read(&c, 1, file_1);
                Write(&c, 1, file_3);
            }
            Close(file_1);
        }
        file_2 = Open("file2.txt", MODE_READ);
        if (file_2 != -1)
        {
            fileSize_2 = Seek(-1, file_2);
            Seek(0, file_2);
            Seek(fileSize_1, file_3);
            i = 0;
            for (; i < fileSize_2; i++)
            {
                Read(&c, 1, file_2);
                Write(&c, 1, file_3);
            }
            Close(file_2);
        }
        Close(file_3);
    }
    Halt();
}