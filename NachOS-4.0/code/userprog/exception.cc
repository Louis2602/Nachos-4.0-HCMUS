// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "ksyscall.h"
#include "main.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------
#define MaxFileLength 32

/**
 * Modify program counter
 * This code is adapted from `../machine/mipssim.cc`, line 667
 **/
void move_program_counter() {
  /* set previous programm counter (debugging only)
   * similar to: registers[PrevPCReg] = registers[PCReg];*/
  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
  /* set programm counter to next instruction
   * similar to: registers[PCReg] = registers[NextPCReg]*/
  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(NextPCReg));
  /* set next programm counter for brach execution
   * similar to: registers[NextPCReg] = pcAfter;*/
  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(NextPCReg) + 4);
}

char *User2System(int virtAddr, int limit) {
  int i; // index
  int oneChar;
  char *kernelBuf = NULL;

  kernelBuf = new char[limit + 1]; // need for terminal string
  if (kernelBuf == NULL)
    return kernelBuf;
  memset(kernelBuf, 0, limit + 1);

  // printf("\n Filename u2s:");
  for (i = 0; i < limit; i++) {
    kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
    kernelBuf[i] = (char)oneChar;
    // printf("%c",kernelBuf[i]);
    if (oneChar == 0)
      break;
  }
  return kernelBuf;
}
int System2User(int virtAddr, int len, char *buffer) {
  if (len < 0)
    return -1;
  if (len == 0)
    return len;
  int i = 0;
  int oneChar = 0;
  do {
    oneChar = (int)buffer[i];
    kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
    i++;
  } while (i < len && oneChar != 0);
  return i;
}

void handle_SC_Halt() {
  DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
  SysHalt();
  ASSERTNOTREACHED();
}
void handle_SC_Create() {
  int virtAddr = kernel->machine->ReadRegister(4);
  char *fileName = User2System(virtAddr, MaxFileLength + 1);

  if (SysCreate(fileName))
    kernel->machine->WriteRegister(2, 0);
  else
    kernel->machine->WriteRegister(2, -1);

  delete[] fileName;
  move_program_counter();
  return;
}
void handle_SC_Open()
{
  int virtAddr = kernel->machine->ReadRegister(4);
  char *fileName = User2System(virtAddr, MaxFileLength + 1);
  // type: OpenForRead - OpenForWrite - OpenForReadWrite
  int type = kernel->machine->ReadRegister(5);
  kernel->machine->WriteRegister(2, SysOpen(fileName, type));

  delete[] fileName;
  move_program_counter();
  return;
}
void handle_SC_Close()
{
  int id = kernel->machine->ReadRegister(4);
  kernel->machine->WriteRegister(2, SysClose(id));
  move_program_counter();
  return;
}
void handle_SC_Read()
{
  move_program_counter();
  return;
}
void handle_SC_Write()
{
  move_program_counter();
  return;
}
void handle_SC_Seek()
{
  move_program_counter();
  return;
}
void handle_SC_Remove()
{
  move_program_counter();
  return;
}
void handle_SC_SocketTCP()
{
  kernel->machine->WriteRegister(2, SysSocketTCP());
  move_program_counter();
  return;
}
void handle_SC_Connect()
{
  int socketid = kernel->machine->ReadRegister(4);
  int virtAddr = kernel->machine->ReadRegister(5);
  char *ip = User2System(virtAddr, MaxFileLength + 1);
  int port = kernel->machine->ReadRegister(6);
  if (socketid == -1)
    kernel->machine->WriteRegister(2, -1);
  else
    kernel->machine->WriteRegister(2, SysConnect(socketid, ip, port));
  move_program_counter();
  return;
}
void handle_SC_Send()
{
  int socketid = kernel->machine->ReadRegister(4);
  int virtAddr = kernel->machine->ReadRegister(5);
  char *buffer = User2System(virtAddr, MaxFileLength + 1);
  int len = kernel->machine->ReadRegister(6);
  if (socketid == -1)
    kernel->machine->WriteRegister(2, -1);
  else
    kernel->machine->WriteRegister(2, SysSend(socketid, buffer, len));
  move_program_counter();
  return;
}
void handle_SC_Receive()
{
  int socketid = kernel->machine->ReadRegister(4);
  // socketid = socket(AF_INET, SOCK_STREAM, 0);
  int virtAddr = kernel->machine->ReadRegister(5);
  char *buffer = User2System(virtAddr, MaxFileLength + 1);
  int len = kernel->machine->ReadRegister(6);
  if (socketid == -1)
    kernel->machine->WriteRegister(2, -1);
  else
    kernel->machine->WriteRegister(2, SysReceive(socketid, buffer, len));
  move_program_counter();
  return;
}
void handle_SC_Add()
{
  DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + "
                       << kernel->machine->ReadRegister(5) << "\n");
  /* Process SysAdd Systemcall*/
  int result;
  result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
                  /* int op2 */ (int)kernel->machine->ReadRegister(5));
  DEBUG(dbgSys, "Add returning with " << result << "\n");
  /* Prepare Result */
  kernel->machine->WriteRegister(2, (int)result);
  move_program_counter();
  return;
  ASSERTNOTREACHED();
}
void ExceptionHandler(ExceptionType which) {
  int type = kernel->machine->ReadRegister(2);

  DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

  switch (which) {
  case NoException:
    kernel->interrupt->setStatus(SystemMode);
    DEBUG(dbgSys, "Switch to system mode\n");
    break;
  case PageFaultException:
    DEBUG('a', "\n No valid translation found");
    printf("\n\n No valid translation found");
    kernel->interrupt->Halt();
    break;
  case ReadOnlyException:
    DEBUG('a', "\n Write attempted to page marked read-only");
    printf("\n\n Write attempted to page marked read-only");
    kernel->interrupt->Halt();
    break;
  case BusErrorException:
    DEBUG('a', "\n Translation resulted invalid physical address");
    printf("\n\n Translation resulted invalid physical address");
    kernel->interrupt->Halt();
    break;
  case AddressErrorException:
    DEBUG('a', "\n Unaligned reference or one that was beyond the end of the "
               "address space");
    printf("\n\n Unaligned reference or one that was beyond the end of the "
           "address space");
    kernel->interrupt->Halt();
    break;
  case OverflowException:
    DEBUG('a', "\nInteger overflow in add or sub.");
    printf("\n\n Integer overflow in add or sub.");
    kernel->interrupt->Halt();
    break;
  case IllegalInstrException:
    DEBUG('a', "\n Unimplemented or reserved instr.");
    printf("\n\n Unimplemented or reserved instr.");
    kernel->interrupt->Halt();
    break;
  case NumExceptionTypes:
    DEBUG('a', "\n Number exception types");
    printf("\n\n Number exception types");
    kernel->interrupt->Halt();
    break;
  case SyscallException:
    switch (type) {
    case SC_Halt:
      return handle_SC_Halt();
    case SC_Add:
      return handle_SC_Add();
    case SC_Create:
      return handle_SC_Create();
    case SC_Open:
      break;
    case SC_Close:
      break;
    case SC_Read:
      break;
    case SC_Write:
      break;
    case SC_Seek:
      break;
    case SC_Remove:
      return handle_SC_Remove();
    case SC_SocketTCP:
      return handle_SC_SocketTCP();
    case SC_Connect:
      return handle_SC_Connect();
    case SC_Send:
      return handle_SC_Send();
    case SC_Receive:
      return handle_SC_Receive();
    default:
      cerr << "Unexpected system call " << type << "\n";
      break;
    }
    break;
  default:
    cerr << "Unexpected user mode exception" << (int)which << "\n";
    break;
  }
  ASSERTNOTREACHED();
}
