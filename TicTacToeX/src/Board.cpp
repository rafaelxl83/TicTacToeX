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
		BoardSizes::TwoPlayers,
		BOARD_DEFAULT_ID,
		anEvtTableId)
{
}

Board::Board(
	unsigned int				anId,
	unsigned int 				anEvtTableId)
	: Board(
		BoardSizes::TwoPlayers,
		anId,
		anEvtTableId)
{
}

Board::Board(
	BoardSizes					theSize,
	unsigned int				anId,
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

bool 
Board::SetMark(
	Point						aPos,
	Symbol						aSymbol
)
{
	// invalid parameters
	if (aSymbol.GetProperty().value == 0)
		return false;
	if (aPos.x >= mySize || aPos.y >= mySize)
		return false;
	// point already marked
	if (theBoard
		[aPos.x + VERIFICATION_ADJUSTMENT]
		[aPos.y + VERIFICATION_ADJUSTMENT].
			GetProperty().value != (int)Symbol::AvailableSymbols::empty)
		return false;

	theBoard
		[aPos.x + VERIFICATION_ADJUSTMENT]
		[aPos.y + VERIFICATION_ADJUSTMENT] = aSymbol;

	markedCells.push_back(Point(aPos.x, aPos.y));
	return true;
}

bool
Board::IsFull()
{
	return markedCells.size() == mySize * mySize;
}

void 
Board::Initialize()
{
	theBoard = std::unique_ptr<Row[]>(new Row[realSize]);
	theSection = std::shared_ptr<Row[]>(new Row[3]);

	for (int i = 0; i < realSize; ++i)
	{
		if(i < 3) theSection[i] = Row(3, 0);
		theBoard[i] = Row(realSize, 0);
	}

	markedCells = std::vector<Point>();
}

std::string
Board::ToString() const
{
	std::string line;
	std::stringbuf buffer;
	std::ostream os(&buffer);

#ifdef _DEBUG
	line.insert(0, mySize * 5, '_');
#else
	line.insert(0, mySize*3, '_');
#endif

	for (int i = VERIFICATION_ADJUSTMENT; i < mySize + VERIFICATION_ADJUSTMENT; i++)
	{
		std::copy(
			theBoard[i].begin() + VERIFICATION_ADJUSTMENT,
			theBoard[i].end() - VERIFICATION_ADJUSTMENT - 1,
			std::ostream_iterator<Symbol>(os, "|"));
		os << (*(theBoard[i].end() - VERIFICATION_ADJUSTMENT - 1))   << std::endl
		   << (i < mySize + VERIFICATION_ADJUSTMENT - 1 ? line : "") << std::endl;
	}

	return "Board - State: "
		+ std::to_string(myId)
		+ "\n"
		+ buffer.str()
		+ '\n';
}