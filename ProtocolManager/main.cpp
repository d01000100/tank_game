#include "ProtocolManager.h"

int main(int argc, char** argv) 
{
	Message* recvM;

	SendBuffer loginSB;
	RecieveBuffer loginRB;
	LoginMessage* login = new LoginMessage();
	login->client_name = "super original";
	loginSB = writeMessage(login);
	//loginRB.buffer = loginSB.buffer;
	loginRB.setDataRecieved(1024);
	recvM = readMessage(loginRB);

	printf("type: %s, name: %s\n", 
		messageTypeString(recvM->type).c_str(),
		((LoginMessage*)recvM)->client_name.c_str());

	SendBuffer joinSB;
	RecieveBuffer joinRecvB;
	JoinMessage* joinM = new JoinMessage();
	joinM->room_name = "server chido";
	joinSB = writeMessage(joinM);
	//joinRecvB.buffer = joinSB.buffer;
	joinRecvB.setDataRecieved(1024);
	recvM = readMessage(joinRecvB);

	printf("type: %s, room: %s\n",
		messageTypeString(recvM->type).c_str(),
		((JoinMessage*)recvM)->room_name.c_str());

	SendBuffer leaveSB;
	RecieveBuffer LeaveRecvB;
	LeaveMessage* LeaveM = new LeaveMessage();
	LeaveM->room_name = "server chido";
	leaveSB = writeMessage(LeaveM);
	//LeaveRecvB.buffer = leaveSB.buffer;
	LeaveRecvB.setDataRecieved(1024);
	recvM = readMessage(LeaveRecvB);

	printf("type: %s, room: %s\n",
		messageTypeString(recvM->type).c_str(),
		((LeaveMessage*)recvM)->room_name.c_str());

	SendBuffer sendSB;
	RecieveBuffer SendRecvB;
	SendMessage* SendM = new SendMessage();
	SendM->room_name = "server chido";
	SendM->message = "todos son super chidos";
	sendSB = writeMessage(SendM);
	//SendRecvB.buffer = sendSB.buffer;
	SendRecvB.setDataRecieved(1024);
	recvM = readMessage(SendRecvB);

	printf("type: %s, room: %s, message: %s\n",
		messageTypeString(recvM->type).c_str(),
		((SendMessage*)recvM)->room_name.c_str(),
		((SendMessage*)recvM)->message.c_str());

	SendBuffer recieveSB;
	RecieveBuffer RecieveRecvB;
	RecieveMessage* RecieveM = new RecieveMessage();
	RecieveM->room_name = "server chido";
	RecieveM->message = "todos son super chidos";
	RecieveM->sender_name = "muy original";
	recieveSB = writeMessage(RecieveM);
	//RecieveRecvB.buffer = recieveSB.buffer;
	RecieveRecvB.setDataRecieved(1024);
	recvM = readMessage(RecieveRecvB);

	printf("type: %s, sender: %s, room: %s, message: %s\n",
		messageTypeString(recvM->type).c_str(),
		((RecieveMessage*)recvM)->sender_name.c_str(),
		((RecieveMessage*)recvM)->room_name.c_str(),
		((RecieveMessage*)recvM)->message.c_str());
}