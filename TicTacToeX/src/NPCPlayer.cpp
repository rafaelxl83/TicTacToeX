#include "NPCPlayer.h"
#include "..\lib\MessageBus.h"

#include "../lib/Utils.h"

#define Calculate(P1,s) s * P1.x + P1.y + 1

#define Convert(P1,P2) \
		Point(P1.x + P2.x - 1,\
			  P1.y + P2.y - 1 )

#define Empty(P1, P2, P3) \
	P1.GetProperty().value == P2.GetProperty().value ? 3 :\
	P2.GetProperty().value == P3.GetProperty().value ? 1 : 2


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
	((std::iostream&)myStream) << 10;
	int mark = Player::MakeAMove();

	myStream.get();
	myStream.clear();
	return mark;
}

void
NPCPlayer::SetBoard(
	std::shared_ptr<Board>		aBoard)
{
	return;
}


int
NPCPlayer::EvaluateBoard()
{
	int s = 0;
	std::vector<Point> marked = theBoard->GetMarkedPositions();

	for (Point p : marked)
	{
		s = CheckSection(p, [this](int v) { return Evaluate(v); });
		if (s != 0)
			break;
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
		switch (Empty(
			aSection[aPos1.x][aPos1.y], 
			aSection[aPos2.x][aPos2.y], 
			aSection[aPos3.x][aPos3.y]))
		{
		case 1: p = Convert(aPoint, aPos1); break;
		case 2: p = Convert(aPoint, aPos2); break;
		case 3: p = Convert(aPoint, aPos3); break;
		}
		sum = Calculate(p, theBoard->GetSize());
	}
	return sum;
}
int
NPCPlayer::Evaluate(
	int							aValue)
{
	switch (aValue - (int)Symbol::AvailableSymbols::empty)
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