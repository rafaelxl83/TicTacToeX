#pragma once

#include <string>

#define DEFAULT_ID			1

class MessageGamePlayBase
{
public:
	MessageGamePlayBase()
		: MessageGamePlayBase(
			DEFAULT_ID)
	{
	}
	MessageGamePlayBase(
		unsigned int anId)
		: callerId(anId)
	{
	}

	unsigned int	callerId;
};

class MessageEndOfGame
	: public MessageGamePlayBase
{
public:
	MessageEndOfGame()
	: MessageGamePlayBase()
	{
	}

	MessageEndOfGame(
		unsigned int anId,
		unsigned int boardID)
	: MessageGamePlayBase(anId)
	{
	}
};

class MessageStartOfGame
	: public MessageGamePlayBase
{
public:
	MessageStartOfGame()
	: MessageGamePlayBase()
	{
	}

	MessageStartOfGame(
		unsigned int anId)
	: MessageGamePlayBase(anId)
	{
	}
};

class MessageTurnChanged
	: public MessageGamePlayBase
{
public:
	MessageTurnChanged()
	: MessageGamePlayBase()
	{
	}

	MessageTurnChanged(
		unsigned int anId)
	: MessageGamePlayBase(anId)
	{
	}
};

class MessageSingleMove
	: public MessageGamePlayBase
{
public:
	MessageSingleMove()
	: MessageGamePlayBase()
	{
	}

	MessageSingleMove(
		unsigned int anId)
	: MessageGamePlayBase(anId)
	{
	}
};

class MessageBlockMove
	: public MessageGamePlayBase
{
public:
	MessageBlockMove()
	: MessageGamePlayBase()
	{
	}

	MessageBlockMove(
		unsigned int anId)
	: MessageGamePlayBase(anId)
	{
	}
};

class MessageScorePoints
	: public MessageGamePlayBase
{
public:
	MessageScorePoints()
	: MessageGamePlayBase()
	{
	}

	MessageScorePoints(
		unsigned int anId)
	: MessageGamePlayBase(anId)
	{
	}
};

class MessageShutdown
	: public MessageGamePlayBase
{
public:
	MessageShutdown()
		: MessageGamePlayBase()
	{
	}
	MessageShutdown(
		unsigned int anId)
		: MessageGamePlayBase(anId)
	{
	}
};