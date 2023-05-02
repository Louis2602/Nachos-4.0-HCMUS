#include "syscall.h"

void main()
{
	int pingPID, pongPID;
	pingPID = Exec("../test/pong");
	pongPID = Exec("../test/ping");
	Join(pingPID);
	Join(pongPID);
	// pongPID = Exec("kha");
  // PrintString("ABCS");
}
