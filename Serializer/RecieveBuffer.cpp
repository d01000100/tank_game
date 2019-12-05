#include "RecieveBuffer.h"

RecieveBuffer::RecieveBuffer(unsigned int size) {
	buffer = new unsigned char[size];
	read_index = 0;
	data_recieved = 0;
}

unsigned char* RecieveBuffer::getBuffer() { return buffer; }

void RecieveBuffer::setDataRecieved(unsigned int len) { 
	data_recieved = len;
}

int RecieveBuffer::readInt() {
	if (data_recieved < read_index + 4) {
		// TODO: Proper error handling
		printf("Buffer empty\n"); 
		return 0;
	}

	int res = this->buffer[read_index];
	res |= this->buffer[read_index + 1] << 8;
	res |= this->buffer[read_index + 2] << 16;
	res |= this->buffer[read_index + 3] << 24;
	read_index += 4;
	return res;
}

short RecieveBuffer::readShort() {
	if (data_recieved < read_index + 2) {
		// TODO: Proper error handling
		printf("Buffer empty\n");
		return 0;
	}

	short res = this->buffer[read_index];
	res |= this->buffer[read_index + 1] << 8;
	read_index += 2;
	return res;
}

std::string RecieveBuffer::readString(unsigned int length) {
	if (data_recieved < read_index + length) {
		// TODO: Proper error handling
		printf("Buffer empty\n");
		return "";
	}

	std::string res;
	for (unsigned int i = 0; i < length; i++) {
		res += buffer[read_index + i];
	}
	read_index += length;
	return res;
}