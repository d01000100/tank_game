#pragma once
#include "Message.h"
#include "SendBuffer.h"
#include "RecieveBuffer.h"

SendBuffer writeMessage(OliMessage* message);
SendBuffer writeMessage(NameMessage* message);
SendBuffer writeMessage(UserInputMessage* message);
SendBuffer writeMessage(GameStateMessage* message);

Message* readMessage(RecieveBuffer buffer);