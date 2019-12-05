
#include "Message.h"

std::string messageTypeString(MessageType t) {
	switch (t)
	{
	case OLI:
		return "OLI";
		break;
	case NAME:
		return "NAME";
		break;
	case USER_INPUT:
		return "USER INPUT";
		break;
	case GAME_STATE:
		return "GAame state";
		break;
	default:
		return "unkonw";
		break;
	}
}

OliMessage::OliMessage() {
	type = OLI;
}

NameMessage::NameMessage() {
	type = NAME;
}

UserInputMessage::UserInputMessage() {
	type = USER_INPUT;
}

GameStateMessage::GameStateMessage() {
	type = GAME_STATE;
}