#include "Board.h"

#include "../lib/MessageBus.h"
#include "ExceptionHelper.h"
#include "GamePlayMessages.h"

#include <cassert>
#include <algorithm>
#include <iterator>
#include <sstream>

Board::Board(
	unsigned int 				anEvtTableId)
	: Board(
		BOARD_DEFAULT_ID,
		BoardSizes::TwoPlayers,
		anEvtTableId)
{
}

Board::Board(
	unsigned int				anId,
	unsigned int 				anEvtTableId)
	: Board(
		anId,
		BoardSizes::TwoPlayers,
		anEvtTableId)
{
}

Board::Board(
	unsigned int				anId,
	BoardSizes					theSize,
	unsigned int 				anEvtTableId)
	: myId(anId)
	, myEvtTableId(anEvtTableId)
{
	Union u;
	u.s = theSize;
	mySize = u.i;
	realSize = mySize + 2 * VERIFICATION_ADJUSTMENT;

	Initialize();

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

unsigned int
Board::GetEvtTableID()
{
	return myEvtTableId;
}

bool 
Board::SetMark(
	Point						aPos,
	Symbol						aSymbol
)
{
	// invalid parameters
	if (aPos.x >= mySize || aPos.y >= mySize)
		return false;

	// point already marked
	if (refHorizontal[aPos.x] && refVertical[aPos.y])
		return false;

	theBoard
		[aPos.x + VERIFICATION_ADJUSTMENT]
		[aPos.y + VERIFICATION_ADJUSTMENT] = aSymbol;

	refHorizontal[aPos.x]	= true;
	refVertical[aPos.y]		= true;

	markedCells.push_back(Point(aPos.x, aPos.y));
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

std::vector<Point>
Board::GetMarkedPositions()
{
	return markedCells;
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

	refHorizontal = std::unique_ptr<bool[]>(new bool[mySize]);
	refVertical = std::unique_ptr<bool[]>(new bool[mySize]);

	markedCells = std::vector<Point>();
}

std::string
Board::ToString() const
{
	return "";
}