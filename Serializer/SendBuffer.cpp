#include "SendBuffer.h"

SendBuffer::SendBuffer(unsigned int size) {
	buffer = new unsigned char[size];
	this->size = size;
	write_index = 0;
}

unsigned char* SendBuffer::getBuffer() { return buffer; }

void SendBuffer::growBuffer() {
	size *= 2;
	unsigned char* newBuffer = new unsigned char[size];
	for (int i = 0; i < write_index; i++) {
		newBuffer[i] = buffer[i];
	}
	delete buffer;
	buffer = newBuffer;
}

bool SendBuffer::writeInt(int data) {
	// While the free space of the buffer is
	// not enought to store the int, grow it
	while (size < write_index + 4) {
		growBuffer();
	}
	buffer[write_index] = data; 
	buffer[write_index + 1] = data >> 8; 
	buffer[write_index + 2] = data >> 16;
	buffer[write_index + 3] = data >> 24;
	write_index += 4;
	return true;
}

bool SendBuffer::writeShort(short data) {
	// While the free space of the buffer is
	// not enought to store the short, grow it
	while (size < write_index + 2) {
		growBuffer();
	}
	buffer[write_index] = data;
	buffer[write_index + 1] = data >> 8;
	write_index += 2;
	return true; 
}

bool SendBuffer::writeString(std::string data) {
	// While the free space of the buffer is
	// not enought to store the string, grow it
	while (size < write_index + data.length()) {
		growBuffer();
	}
	for (unsigned int i = 0; i < data.length(); i++) {
		buffer[write_index + i] = data[i];
	}
	write_index += data.length();
	return true; 
}


void SendBuffer::printBuffer() {
	for (int i = 0; i < write_index; i++) {
		printf("%X ", buffer[i]);
	}
	printf("\n");
}

unsigned int SendBuffer::getDataLength() {
	return write_index; // in bytes
}
