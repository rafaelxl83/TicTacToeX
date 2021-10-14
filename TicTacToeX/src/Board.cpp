#include "Board.h"

#include "../lib/MessageBus.h"
#include "ExceptionHelper.h"
#include "GamePlayMessages.h"

#include <cassert>
#include <algorithm>
#include <iterator>
#include <sstream>

Board::Board(
	std::shared_ptr<EventTable>	anEvtTable)
	: Board(
		BOARD_DEFAULT_ID,
		BoardSizes::TwoPlayers,
		anEvtTable)
{
}

Board::Board(
	unsigned int				anId,
	std::shared_ptr<EventTable>	anEvtTable)
	: Board(
		anId,
		BoardSizes::TwoPlayers,
		anEvtTable)
{
}

Board::Board(
	unsigned int				anId,
	BoardSizes					theSize,
	std::shared_ptr<EventTable>	anEvtTable)
	: myId(anId)
	, myEvtTable(anEvtTable)
{
	Union u;
	u.s = theSize;
	mySize = u.i;
	realSize = mySize + 2 * VERIFICATION_ADJUSTMENT;
	Log("[Board]", myId, "Created", ToString());
}

Board::~Board()
{
}

unsigned int 
Board::GetID()
{
	return myId;
}

std::shared_ptr<EventTable>
Board::GetEvtTable()
{
	return myEvtTable;
}

bool 
Board::SetMark(
	Point						aPos,
	Symbol						aSymbol
)
{
	if (aPos.x >= mySize || aPos.y >= mySize)
		return false;

	theBoard
		[aPos.x + VERIFICATION_ADJUSTMENT]
		[aPos.y + VERIFICATION_ADJUSTMENT] = aSymbol;

	return true;
}

std::weak_ptr<Row[]> 
Board::GetSector(
	Point						aPos
)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			theSection[i][j] = theBoard
				[aPos.x + VERIFICATION_ADJUSTMENT + i - 1]
				[aPos.y + VERIFICATION_ADJUSTMENT + j - 1];

	return theSection;
}

void 
Board::Initialize()
{
	theBoard = std::unique_ptr<Row[]>(new Row[realSize]);
	std::shared_ptr<Row[]> theSection = std::shared_ptr<Row[]>(new Row[3]);

	for (int i = 0; i < realSize; ++i)
	{
		if(i < 3) theSection[i] = Row(3, 0);
		theBoard[i] = Row(realSize, 0);
	}
}

std::string
Board::ToString() const
{
	return "";
}