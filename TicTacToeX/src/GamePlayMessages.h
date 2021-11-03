#pragma once

#include <string>

#define DEFAULT_ID			1

class MessageGamePlayBase
{
public:
	MessageGamePlayBase()
		: MessageGamePlayBase(
			DEFAULT_ID, 0, 0)
	{
	}
	MessageGamePlayBase(
		unsigned int anId)
		: MessageGamePlayBase(
			anId, 0, 0)
	{
	}
	MessageGamePlayBase(
		unsigned int aPlayerId,
		unsigned int aBoardId)
		: MessageGamePlayBase(
			DEFAULT_ID, 
			aPlayerId,
			aBoardId)
	{
	}
	MessageGamePlayBase(
		unsigned int anId,
		unsigned int aPlayerId,
		unsigned int aBoardId)
		: callerId(anId)
		, myPlayerId(aPlayerId)
		, myBoardId(aBoardId)
	{
	}

	unsigned int	callerId;
	unsigned int	myPlayerId;
	unsigned int	myBoardId;
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
		unsigned int anId)
		: MessageGamePlayBase(anId)
	{
	}

	MessageEndOfGame(
		unsigned int aPlayerId,
		unsigned int aBoardId)
		: MessageGamePlayBase(
			aPlayerId, aBoardId)
	{
	}

	MessageEndOfGame(
		unsigned int anId,
		unsigned int aPlayerId,
		unsigned int aBoardId)
	: MessageGamePlayBase(
		anId, aPlayerId, aBoardId)
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

	MessageTurnChanged(
		unsigned int anId,
		unsigned int aBoardId)
		: MessageGamePlayBase(
			anId, aBoardId)
	{
	}

	MessageTurnChanged(
		int aTurnIdx)
		: MessageGamePlayBase()
		, turn(aTurnIdx)
	{
	}

	MessageTurnChanged(
		unsigned int anId,
		int aTurnIdx)
		: MessageGamePlayBase(anId)
		, turn(aTurnIdx)
	{
	}

	MessageTurnChanged(
		unsigned int anId,
		unsigned int aBoardId,
		int aTurnIdx)
		: MessageGamePlayBase(
			anId, aBoardId)
		, turn(aTurnIdx)
	{
	}

	int turn;
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

	MessageSingleMove(
		unsigned int anId,
		unsigned int aBoardId)
		: MessageGamePlayBase(
			anId, aBoardId)
	{
	}

	MessageSingleMove(
		int thePlayerMark)
		: MessageGamePlayBase()
		, mark(thePlayerMark)
	{
	}

	MessageSingleMove(
		unsigned int anId,
		int thePlayerMark)
		: MessageGamePlayBase(anId)
		, mark(thePlayerMark)
	{
	}

	MessageSingleMove(
		unsigned int anId,
		unsigned int aBoardId,
		int thePlayerMark)
		: MessageGamePlayBase(
			anId, aBoardId)
		, mark(thePlayerMark)
	{
	}

	int mark;
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