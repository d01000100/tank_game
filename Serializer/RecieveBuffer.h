#pragma once
#include <string>
class RecieveBuffer
{
private:
	unsigned int read_index;
	unsigned int data_recieved;
public:
	unsigned char* buffer; 
	RecieveBuffer(unsigned int size = 1024);
	unsigned char* getBuffer(); // to expose the buffer to the socket
	void setDataRecieved(unsigned int len);
	int readInt();
	short readShort();
	std::string readString(unsigned int length);
};

