#include "Board.h"
#include "ExceptionHelper.h"

#include <cassert>
#include <algorithm>
#include <iterator>
#include <sstream>

Board::Board(
	unsigned int 				anId)
	: Board(
		BoardSizes::TwoPlayers,
		anId)
{
}

Board::Board(
	BoardSizes					theSize,
	unsigned int				anId)
	: myId(anId)
{
	mySize = (short)theSize;
	realSize = mySize + 2 * VERIFICATION_ADJUSTMENT;
	cellsAmount = mySize * mySize;

	Initialize();

	Log("[Board]", myId, "Created", ToString());
}

short
Board::GetSize()
{
	return mySize;
}

unsigned int 
Board::GetID()
{
	return myId;
}

std::optional<Point>
Board::GetPoint(
	short						aPos)
{
	if (1 <= aPos && aPos <= cellsAmount)
		return boardMap.at(aPos);

	return std::nullopt;
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
Board::SetMark(
	short						aPos,
	Symbol						aSymbol)
{
	if (1 <= aPos && aPos <= cellsAmount)
		return SetMark(boardMap.at(aPos), aSymbol);

	return false;
}

bool
Board::IsFull()
{
	return markedCells.size() == cellsAmount;
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

	int k = 1;
	for (int i = 0; i < mySize; i++)
		for (int j = 0; j < mySize; j++)
			boardMap.insert(std::pair<short, Point>(k++, Point(i, j)));

	markedCells = std::vector<Point>();
}

void
Board::PrintBoard(
	std::ostream&				aStream)
{
	std::string line;

	line.insert(0, 11 + (mySize - 3) * 4, '-');
	for (int i = 0; i < mySize - 1; i++)
		line[i * 4 + 3] = '|';
	line = "\n" + line + "\n";

	for (std::pair<int, Point> p : boardMap)
	{
		Symbol s = theBoard
			[p.second.x + VERIFICATION_ADJUSTMENT]
			[p.second.y + VERIFICATION_ADJUSTMENT];
		
			if (s.GetProperty().symbol == Symbol::AvailableSymbols::empty)
				aStream << " " << p.first << (p.first < 10 ? " " : "");
			else 
				aStream << " " << s.GetProperty().ico << " ";
			if(p.first < cellsAmount)
				aStream << (p.first % mySize == 0 ? line : "|");
	}
}

std::string
Board::ToString() const
{
	std::string line;
	std::stringbuf buffer;
	std::ostream os(&buffer);

#ifdef _DEBUG
	line.insert(0, mySize * 5, '-');
#else
	line.insert(0, mySize*3, '-');
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