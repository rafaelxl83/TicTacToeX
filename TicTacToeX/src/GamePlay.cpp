#include "GamePlay.h"

#include "../lib/MessageBus.h"

#include "ExceptionHelper.h"

GamePlay::GamePlay()
{

}
GamePlay::GamePlay(
	std::list<Board>				allBoards)
{

}
GamePlay::~GamePlay()
{

}

#pragma region "Message Events"
void
GamePlay::OnMessageEndOfGame(
	const MessageEndOfGame&			aMessage) noexcept
{
	try
	{
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageEndOfGame", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageStartOfGame(
	const MessageStartOfGame&		/*aMessage*/) noexcept
{
	try
	{
		SEND_TO_GAMEPLAY(MessageTurnChanged());
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageStartOfGame", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageTurnChanged(
	const MessageTurnChanged&		aMessage) noexcept
{
	try
	{
		if (!shutDown)
		{
			Turn();
			SEND_TO_PLAYERS(MessageSingleMove());
		}
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageTurnChanged", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageSingleMove(
	const MessageSingleMove&		aMessage) noexcept
{
	try
	{
		if (!shutDown)
		{
			
		}
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageSingleMove", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageBlockMove(
	const MessageBlockMove&			aMessage) noexcept
{
	try
	{
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageBlockMove", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageScorePoints(
	const MessageScorePoints&		aMessage) noexcept
{
	try
	{
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageScorePoints", "Exception", ex.what());
	}
}

void
GamePlay::OnMessageShutdown(
	const MessageShutdown&			aMessage) noexcept
{
	try
	{
		ShutDown();
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageShutdown", "Exception", ex.what());
	}
}
#pragma endregion

void
GamePlay::Start()
{
	REGISTER_BOARD(MessageShutdown,			GamePlay::OnMessageShutdown);
	REGISTER_BOARD(MessageEndOfGame,		GamePlay::OnMessageEndOfGame);
	REGISTER_BOARD(MessageStartOfGame,		GamePlay::OnMessageStartOfGame);
	REGISTER_BOARD(MessageTurnChanged,		GamePlay::OnMessageTurnChanged);
	REGISTER_BOARD(MessageSingleMove,		GamePlay::OnMessageSingleMove);
	REGISTER_BOARD(MessageBlockMove,		GamePlay::OnMessageBlockMove);
	REGISTER_BOARD(MessageScorePoints,		GamePlay::OnMessageScorePoints);

	Initialize();

	SEND_TO_GAMEPLAY(MessageStartOfGame());
	SEND_TO_PLAYERS(MessageStartOfGame());
}

void
GamePlay::ShutDown()
{
	shutDown = true;
}

void
GamePlay::AddBoard(
	Board*						aBoard)
{
	myBoards.push_back(aBoard);
}
void
GamePlay::SetBoards(
	std::vector<Board*>			allBoards)
{
	//myBoards = std::move(allBoards); //old method
	std::ranges::move(allBoards, std::back_inserter(myBoards));
}

Board& 
GamePlay::GetBoard(
	int							anIndex)
{
	return *myBoards[anIndex];
}
Board&
GamePlay::GetBoard(
	unsigned int				aBoardId)
{
	auto board = std::ranges::find_if(
		myBoards,
		[aBoardId](Board* b) { return (b->GetID() == aBoardId); });

	if (board != myBoards.end())
		return **board;

	return *myBoards[0];
}

std::string
GamePlay::PrintBoard()
{
	
	return "";
}

void
GamePlay::Initialize()
{
	for (auto b : myBoards)
	{
		if ((int)BoardSizes::TwoPlayers > b->GetSize() || b->GetSize() > (int)BoardSizes::FivePlayers)
		{
			throw std::exception(
				"Invalid number of players, must be between 2 and 5 players.");
		}
	}

}

void
GamePlay::Turn()
{

}

Symbol
GamePlay::HasWinner(
	Board&						aBoard)
{
	// There are no winners before the 5th mark
	if (aBoard.GetMarkedPositions().size() < 5)
		return Symbol(0);

	int s = aBoard.GetSize() - 1;
	std::weak_ptr<Row[]> aSection;
	std::vector<Point> marked;
	std::vector<Point> ignore = {Point(), Point(0,s), Point(s,0), Point(s,s)};

	// No need to check corners
	std::ranges::copy_if(
		aBoard.GetMarkedPositions(),
		std::back_inserter(marked),
		[ignore](Point& p)
		{
			return (std::ranges::find(ignore, p) == ignore.end());
		}
	);

	for (Point p : marked)
	{
		s = CheckSection(aBoard, p);
		if (s != 0)
			break;
	}

	return Symbol(s);
}

int
GamePlay::CheckSection(
	Board& aBoard,
	Point						aPoint)
{
	int sum = 0;
	std::shared_ptr<Row[]> aSection = aBoard.GetSector(aPoint).lock();

#pragma region "Horizontal"
	sum = aSection[0][0].GetProperty().value + 
		  aSection[0][1].GetProperty().value + 
		  aSection[0][2].GetProperty().value;
	sum = Evaluate(sum);
	Log("[GamePlay]", "CheckSection H1", sum);
	if (sum > 0) return sum;

	sum = aSection[1][0].GetProperty().value + 
		  aSection[1][1].GetProperty().value + 
		  aSection[1][2].GetProperty().value;
	sum = Evaluate(sum);
	Log("[GamePlay]", "CheckSection H2", sum);
	if (sum > 0) return sum;

	sum = aSection[2][0].GetProperty().value + 
		  aSection[2][1].GetProperty().value + 
		  aSection[2][2].GetProperty().value;
	sum = Evaluate(sum);
	Log("[GamePlay]", "CheckSection H3", sum);
	if (sum > 0) return sum;
#pragma endregion

#pragma region "Vertical"
	sum = aSection[0][0].GetProperty().value + 
		  aSection[1][0].GetProperty().value + 
		  aSection[2][0].GetProperty().value;
	sum = Evaluate(sum);
	Log("[GamePlay]", "CheckSection V1", sum);
	if (sum > 0) return sum;

	sum = aSection[0][1].GetProperty().value + 
		  aSection[1][1].GetProperty().value + 
		  aSection[2][1].GetProperty().value;
	sum = Evaluate(sum);
	Log("[GamePlay]", "CheckSection V2", sum);
	if (sum > 0) return sum;

	sum = aSection[0][2].GetProperty().value + 
		  aSection[1][2].GetProperty().value + 
		  aSection[2][2].GetProperty().value;
	sum = Evaluate(sum);
	Log("[GamePlay]", "CheckSection V3", sum);
	if (sum > 0) return sum;
#pragma endregion

#pragma region "Diagonal"
	sum = aSection[0][0].GetProperty().value + 
		  aSection[1][1].GetProperty().value + 
		  aSection[2][2].GetProperty().value;
	sum = Evaluate(sum);
	Log("[GamePlay]", "CheckSection D1", sum);
	if (sum > 0) return sum;

	sum = aSection[0][2].GetProperty().value + 
		  aSection[1][1].GetProperty().value + 
		  aSection[2][0].GetProperty().value;
	sum = Evaluate(sum);
	Log("[GamePlay]", "CheckSection D2", sum);
	if (sum > 0) return sum;
#pragma endregion

	return 0;
}

int
GamePlay::Evaluate(
	int							aValue)
{
	switch (aValue)
	{
		case (int)Symbol::AvailableSymbols::X * 3:
			return 1;
		case (int)Symbol::AvailableSymbols::O * 3:
			return 2;
		case (int)Symbol::AvailableSymbols::M * 3:
			return 3;
		case (int)Symbol::AvailableSymbols::S * 3:
			return 4;
		case (int)Symbol::AvailableSymbols::H * 3:
			return 5;
		default:
			return 0;
	}
}