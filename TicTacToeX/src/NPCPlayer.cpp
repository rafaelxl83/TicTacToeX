#include "NPCPlayer.h"
#include "..\lib\MessageBus.h"

#include "../lib/Utils.h"

#define Calculate(P1,s) s * P1.x + P1.y + 1

#define Convert(P1,P2) \
		Point(P1.x + P2.x - 1,\
			  P1.y + P2.y - 1 )

#define Empty(P1, P2, P3) \
	P1.GetProperty().value == (int)Symbol::AvailableSymbols::empty ? 1 :\
	P2.GetProperty().value == (int)Symbol::AvailableSymbols::empty ? 2 :\
	P3.GetProperty().value == (int)Symbol::AvailableSymbols::empty ? 3 : 0


NPCPlayer::NPCPlayer(
	unsigned int				anId,
	std::string					aNickName,
	Symbol						aSymbol)
	: Player(
		anId, 
		aNickName, 
		aSymbol,
		*(new std::iostream(&buffer)))
{
	
}

unsigned int
NPCPlayer::MakeAMove()
{
	unsigned int mark = NO_MARK;
	/*((std::iostream&)myStream) << 10;
	mark = Player::MakeAMove();

	myStream.get();
	myStream.clear();*/
	
	if (myState == PlayerState::Turn)
	{
		mark = EvaluateBoard();
		if (mark == 0)
		{
			int upLimit = theBoard->GetSize();
			upLimit *= upLimit;
			mark = Random(1, upLimit);
		}
	}

	return mark;
}

void
NPCPlayer::SetBoard(
	Board*						aBoard)
{
	theBoard = std::shared_ptr<Board>(aBoard);

	int bSize = theBoard->GetSize() - 2;
	int verifSize = bSize * bSize;
	
	for (int k = 0, i = 1, j = 1; k < verifSize; k++)
	{
		cells2Verify.push_back(Point(i, j));
		(j < bSize ? j++ : (j = 1, i++));
	}
}

int
NPCPlayer::EvaluateBoard()
{
	if (theBoard->GetMarkedPositions().size() <= 3)
		return 0;

	int s = 0;
	for (Point p : cells2Verify)
	{
		// First check the opportunity to be the winner
		s = CheckSection(p, [this](int v) { return EvaluateMe(v); });
		if (s != 0) break;
		else
		{
			// Check if there are any opponent victory menace
			s = CheckSection(p, [this](int v) { return Evaluate(v); });
			if (s != 0)
				break;
		}
	}

	return s;
}
int
NPCPlayer::CheckSection(
	Point						aPoint,
	std::function<int (int)>	eValuation)
{
	int sum = 0;
	Point p;
	std::shared_ptr<Row[]> aSection = 
		theBoard->GetSector(aPoint).lock();

#pragma region "Horizontal"
	sum = CheckLine(Point(0, 0), Point(0, 1), Point(0, 2), 
		aPoint, aSection, eValuation);
	if (sum > 0)
	{
		Log("[NPCPlayer]", "CheckSection H1", sum);
		return sum;
	}

	sum = CheckLine(Point(1, 0), Point(1, 1), Point(1, 2), 
		aPoint, aSection, eValuation);
	if (sum > 0)
	{
		Log("[NPCPlayer]", "CheckSection H2", sum);
		return sum;
	}

	sum = CheckLine(Point(2, 0), Point(2, 1), Point(2, 2),
		aPoint, aSection, eValuation);
	if (sum > 0)
	{
		Log("[NPCPlayer]", "CheckSection H3", sum);
		return sum;
	}
#pragma endregion

#pragma region "Vertical"
	sum = CheckLine(Point(0, 0), Point(1, 0), Point(2, 0),
		aPoint, aSection, eValuation);
	if (sum > 0)
	{
		Log("[NPCPlayer]", "CheckSection V1", sum);
		return sum;
	}

	sum = CheckLine(Point(0, 1), Point(1, 1), Point(2, 1),
		aPoint, aSection, eValuation);
	if (sum > 0)
	{
		Log("[NPCPlayer]", "CheckSection V2", sum);
		return sum;
	}

	sum = CheckLine(Point(0, 2), Point(1, 2), Point(2, 2),
		aPoint, aSection, eValuation);
	if (sum > 0)
	{
		Log("[NPCPlayer]", "CheckSection V3", sum);
		return sum;
	}
#pragma endregion

#pragma region "Diagonal"
	sum = CheckLine(Point(0, 0), Point(1, 1), Point(2, 2),
		aPoint, aSection, eValuation);
	if (sum > 0)
	{
		Log("[NPCPlayer]", "CheckSection D1", sum);
		return sum;
	}

	sum = CheckLine(Point(0, 2), Point(1, 1), Point(2, 0),
		aPoint, aSection, eValuation);
	if (sum > 0)
	{
		Log("[NPCPlayer]", "CheckSection D2", sum);
		return sum;
	}
#pragma endregion

	return 0;
}
int
NPCPlayer::CheckLine(
	Point						aPos1,
	Point						aPos2,
	Point						aPos3,
	Point						aPoint,
	std::shared_ptr<Row[]>		aSection,
	std::function<int (int)>	eValuation)
{
	Point p;
	int sum = 
		aSection[aPos1.x][aPos1.y].GetProperty().value +
		aSection[aPos2.x][aPos2.y].GetProperty().value +
		aSection[aPos3.x][aPos3.y].GetProperty().value;

	sum = eValuation(sum);
	if (sum > 0)
	{
		// There are a situation where the opponent
		// can win  the match or the current NPC 
		// player can be the winner.
		// find the correct spot
		switch (Empty(
			aSection[aPos1.x][aPos1.y], 
			aSection[aPos2.x][aPos2.y], 
			aSection[aPos3.x][aPos3.y]))
		{
			case 1: p = Convert(aPoint, aPos1); break;
			case 2: p = Convert(aPoint, aPos2); break;
			case 3: p = Convert(aPoint, aPos3); break;
			default: return 0;
		}

		// return the spot position to avoid a 
		// winner or to be the winner
		int size = theBoard->GetSize();
		sum = Calculate(p, size);

		if (0 >= sum || sum > size * size)
			return 0;
	}
	return sum;
}
int
NPCPlayer::Evaluate(
	int							aValue)
{
	if (aValue >= NO_NEIGHBOR)
		return 0;

	int currVal = aValue - (int)Symbol::AvailableSymbols::empty;
	if (mySymbol.GetProperty().value * 2 == currVal)
		return 0;

	switch (currVal)
	{
	case (int)Symbol::AvailableSymbols::X * 2:
		return 1;
	case (int)Symbol::AvailableSymbols::O * 2:
		return 2;
	case (int)Symbol::AvailableSymbols::M * 2:
		return 3;
	case (int)Symbol::AvailableSymbols::S * 2:
		return 4;
	case (int)Symbol::AvailableSymbols::H * 2:
		return 5;
	default:
		return 0;
	}
}
int
NPCPlayer::EvaluateMe(
	int							aValue)
{
	if (aValue >= NO_NEIGHBOR)
		return 0;

	int currVal = aValue - (int)Symbol::AvailableSymbols::empty;
	if (mySymbol.GetProperty().value * 2 == currVal)
		return 1;

	return 0;
}