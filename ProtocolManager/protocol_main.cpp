#include "ProtocolManager.h"

int main()
{
	SendBuffer sendBuf = writeMessage(new OliMessage());

	RecieveBuffer recvBuf;
	recvBuf.buffer = sendBuf.buffer;
	recvBuf.setDataRecieved(1024);

	OliMessage* readOli = (OliMessage*)readMessage(recvBuf);

	printf("read Oli's type: %s\n", messageTypeString(readOli->type));

	return 0;
}