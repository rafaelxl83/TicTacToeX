#include "GamePlay.h"

#include "../lib/MessageBus.h"

#include "ExceptionHelper.h"

GamePlay::GamePlay()
{

}
GamePlay::GamePlay(
	std::list<Player>				allPlayers)
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
			SEND_TO_GAMEPLAY(MessageSingleMove());
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
			myPlayers[myTurnCounter - 1]->MakeAMove();
			SEND_TO_GAMEPLAY(MessageTurnChanged());
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
}

void
GamePlay::ShutDown()
{
	shutDown = true;
}

void
GamePlay::AddPlayer(
	Player* aPlayer)
{
	myPlayers.push_back(aPlayer);
}
void
GamePlay::SetPlayers(
	std::vector<Player>			allPlayers)
{

}

std::string
GamePlay::PrintBoard()
{
	for (std::pair<int, Point> p : boardMap)
	{
		std::cout << "<" << p.first << ": " << p.second << ">" << std::endl;
	}

	return "";
}

void
GamePlay::Initialize()
{
	int boardSize = myPlayersCount  = myPlayers.size();
	if ((int)BoardSizes::TwoPlayers < boardSize || boardSize > (int)BoardSizes::FivePlayers)
	{
		throw std::exception(
			"Invalid number of players, must be between 2 and 5 players.");
	}

	int k = 1;
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++)
			boardMap.insert(std::pair<short, Point>(k++, Point(i, j)));

	myTurnCounter = 1;
	for(Player* p: myPlayers)
		p->SetSymbol(Symbol(myTurnCounter++));

}

void
GamePlay::Turn()
{

	if (myTurnCounter == myPlayersCount)
		myTurnCounter = 1;

	myPlayers[myTurnCounter++]->SetState(Player::PlayerState::Idle);
	myPlayers[myTurnCounter]->SetState(Player::PlayerState::Turn);
}