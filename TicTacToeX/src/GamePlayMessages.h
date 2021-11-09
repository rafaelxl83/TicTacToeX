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
		unsigned int anId,
		int aWinner)
		: MessageGamePlayBase(anId)
		, theWinner(aWinner)
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
		unsigned int aBoardId,
		int aWinner)
		: MessageGamePlayBase(
			anId, aBoardId)
		, theWinner(aWinner)
	{
	}

	int theWinner = 0;
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

	MessageTurnChanged(
		unsigned int anId,
		unsigned int aPlayerId,
		unsigned int aBoardId,
		int aTurnIdx)
		: MessageGamePlayBase(
			anId, aPlayerId, aBoardId)
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
		int thePlayerMark,
		int theSymbol)
		: MessageGamePlayBase()
		, mark(thePlayerMark)
		, symbol(theSymbol)
	{
	}

	MessageSingleMove(
		unsigned int anId,
		int thePlayerMark,
		int theSymbol)
		: MessageGamePlayBase(anId)
		, mark(thePlayerMark)
		, symbol(theSymbol)
	{
	}

	MessageSingleMove(
		unsigned int anId,
		unsigned int aBoardId,
		int thePlayerMark,
		int theSymbol)
		: MessageGamePlayBase(
			anId, aBoardId)
		, mark(thePlayerMark)
		, symbol(theSymbol)
	{
	}

	MessageSingleMove(
		unsigned int anId,
		unsigned int aPlayerId,
		unsigned int aBoardId,
		int thePlayerMark,
		int theSymbol)
		: MessageGamePlayBase(
			anId, aPlayerId, aBoardId)
		, mark(thePlayerMark)
		, symbol(theSymbol)
	{
	}

	int mark;
	int symbol;
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
		unsigned int anId,
		int thePoints,
		int theSymbol)
		: MessageGamePlayBase(anId)
		, points(thePoints)
		, symbol(theSymbol)
	{
	}

	MessageScorePoints(
		unsigned int anId,
		unsigned int aPlayerId,
		unsigned int aBoardId,
		int thePoints,
		int theSymbol)
		: MessageGamePlayBase(
			anId, aPlayerId, aBoardId)
		, points(thePoints)
		, symbol(theSymbol)
	{
	}

	int points;
	int symbol;
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