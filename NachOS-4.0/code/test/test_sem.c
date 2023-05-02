#include "syscall.h"
int main() {
    int f_success = CreateSemaphore("testSem", 0);
    if (f_success == -1) {
        PrintString("CreateSemaphore failed\n");
        return 1;
    }
    Wait("testSem");
    Signal("testSem");
}