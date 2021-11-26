#pragma once

#include <string>

#define DEFAULT_ID			1

enum class ScorePoints : int
{
	WINNER = 100,
	SMOVE = 5,
	BMOVE = 15,
	LOOSE = -5
};

enum class TurnActions : int
{
	Next = 0,
	Retake,
	None
};

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
		unsigned int aBoardId)
		: MessageGamePlayBase(
			anId, 0, aBoardId)
	{
	}

	MessageEndOfGame(
		unsigned int aPlayerId,
		unsigned int aBoardId,
		int aWinner)
		: MessageGamePlayBase(
			aPlayerId, aBoardId)
		, theWinner(aWinner)
	{
	}

	MessageEndOfGame(
		unsigned int anId,
		unsigned int aPlayerId,
		unsigned int aBoardId,
		int aWinner)
		: MessageGamePlayBase(
			anId, aPlayerId, aBoardId)
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
		unsigned int aPlayerId,
		unsigned int aBoardId)
		: MessageGamePlayBase(
			aPlayerId, aBoardId)
	{
	}

	MessageStartOfGame(
		unsigned int anId)
		: MessageGamePlayBase(anId)
	{
	}

	MessageStartOfGame(
		unsigned int anId,
		unsigned int aPlayerId,
		unsigned int aBoardId)
		: MessageGamePlayBase(
			anId, aPlayerId, aBoardId)
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

	int turn = 0;
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

	int mark = 0;
	int symbol = 0;
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

	int points = 0;
	int symbol = 0;
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

	MessageBlockMove(
		unsigned int anId,
		int thePoints,
		int theSymbol)
		: MessageGamePlayBase(anId)
		, points(thePoints)
		, symbol(theSymbol)
	{
	}

	MessageBlockMove(
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

	int points = 0;
	int symbol = 0;
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