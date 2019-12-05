#include "SendBuffer.h"
#include "RecieveBuffer.h"

// For debugging
int main(int argc, char** argv) {
	SendBuffer sendBuffer;
	int a = 46546456;
	printf("%d\n", a);
	sendBuffer.writeInt(a);

	a = -727272727;
	printf("%d\n", a);
	sendBuffer.writeInt(a);

	short b = -44444;
	printf("%d\n", b);
	sendBuffer.writeShort(b);

	std::string s = "ke ase";

	//for (int i = 0; i < s.length(); i++) {
	//	printf("%d ", s[i]);
	//}
	printf("%s\n", s.c_str());
	sendBuffer.writeString(s);

	//sendBuffer.printBuffer();

	RecieveBuffer recvB;
	//recvB.buffer = sendBuffer.buffer;
	recvB.setDataRecieved(16);

	printf("read: %d\n", recvB.readInt());
	printf("read: %d\n", recvB.readInt());
	printf("read: %d\n", recvB.readShort());
	printf("read: %s\n", recvB.readString(s.length()).c_str());
	printf("read: %s\n", recvB.readString(s.length()).c_str());
}
