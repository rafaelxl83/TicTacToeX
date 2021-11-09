#include "Players.h"

#include "../lib/MessageBus.h"

#include "ExceptionHelper.h"

#pragma region "Message Events"
void
Players::OnMessageEndOfGame(
	const MessageEndOfGame& aMessage) noexcept
{
	try
	{
		if (done || aMessage.callerId != myId)
			return;

		// check if there are winner or it is a tie
	}
	catch (std::system_error& ex)
	{
		Log("[Players]", "OnMessageEndOfGame", "Exception", ex.what());
	}
}
void
Players::OnMessageStartOfGame(
	const MessageStartOfGame& aMessage) noexcept
{
	try
	{
		if (done || aMessage.callerId != myId)
			return;

		// call the turn of events
		SEND_TO_PLAYERS(MessageTurnChanged(
			aMessage.callerId));
	}
	catch (std::system_error& ex)
	{
		Log("[Players]", "OnMessageStartOfGame", "Exception", ex.what());
	}
}
void
Players::OnMessageTurnChanged(
	const MessageTurnChanged& aMessage) noexcept
{
	try
	{
		if (done || aMessage.callerId != myId)
			return;

		// get the first available player to make a movement
		// all others must be with the idle state
		// with the correct player listed must send the message
		// to perform a movement
		SEND_TO_PLAYERS(MessageSingleMove(
			aMessage.callerId,
			aMessage.myPlayerId,
			aMessage.myBoardId, 0
		));
	}
	catch (std::system_error& ex)
	{
		Log("[Players]", "OnMessageTurnChanged", "Exception", ex.what());
	}
}
void
Players::OnMessageSingleMove(
	const MessageSingleMove& aMessage) noexcept
{
	try
	{
		if (done || aMessage.callerId != myId)
			return;

		// when called it should get the proper player
		// index/id from the message and collect the input
		// from this respective player
		std::optional<Player*> player = GetPlayer(aMessage.myPlayerId);
		if (player.has_value())
		{
			int mark = GetInput(aMessage.myPlayerId);
			int symbol = (int)player.value()->
				GetPlayerSymbol().
				GetProperty().symbol;

			SEND_TO_GAMEPLAY(MessageSingleMove(
				aMessage.callerId,
				aMessage.myPlayerId,
				aMessage.myBoardId,
				mark, symbol
			));
		}
	}
	catch (std::system_error& ex)
	{
		Log("[Players]", "OnMessageSingleMove", "Exception", ex.what());
	}
}
#pragma endregion

#pragma region "Commom methods"
Players::Players()
	: Players(
		PLAYERS_DEFAULT_ID,
		std::vector<Player*>())
{

}
Players::Players(
	const unsigned int			anId)
	: Players(
		anId,
		std::vector<Player*>())
{

}
Players::Players(
	std::vector<Player*>		allPlayers)
	: Players(
		PLAYERS_DEFAULT_ID,
		allPlayers)
{

}
Players::Players(
	const unsigned int			anId,
	std::vector<Player*>		allPlayers)
	: myId(anId)
	, myPlayers(std::move(allPlayers))
{

}
Players::~Players()
{

}

void
Players::Start()
{
	REGISTER_PLAYER(MessageEndOfGame,		Players::OnMessageEndOfGame);
	REGISTER_PLAYER(MessageStartOfGame,		Players::OnMessageStartOfGame);
	REGISTER_PLAYER(MessageTurnChanged,		Players::OnMessageTurnChanged);
	REGISTER_PLAYER(MessageSingleMove,		Players::OnMessageSingleMove);

	Initialize();
}

void
Players::Done()
{
	done = true;
}

void
Players::Initialize()
{

}

void
Players::Turn()
{

}
#pragma endregion

#pragma region "Player methods"
void
Players::AddPlayer(
	Player*						aPlayer)
{
	myPlayers.push_back(aPlayer);
}
void
Players::AddPlayers(
	std::vector<Player*>		allPlayers)
{
	std::ranges::move(allPlayers, std::back_inserter(myPlayers));
}

std::optional<Player*>
Players::GetPlayer(
	int							anIndex)
{
	return std::make_optional(myPlayers[anIndex]);
}
std::optional<Player*>
Players::GetPlayer(
	unsigned int				aPlayerId)
{
	auto player = std::ranges::find_if(
		myPlayers,
		[aPlayerId](Player* p) { return (p->GetID() == aPlayerId); });

	if (player != myPlayers.end())
		return std::make_optional(*player);

	return std::nullopt;
}
std::vector<Player*>
Players::GetAllPlayer()
{
	return myPlayers;
}

unsigned int
Players::GetInput(
	int							anIndex)
{
	std::optional<Player*> player = GetPlayer(anIndex);
	if (player.has_value())
		return player.value()->MakeAMove();

	return false;
}
unsigned int
Players::GetInput(
	unsigned int				aPlayerId)
{
	std::optional<Player*> player = GetPlayer(aPlayerId);
	if (player.has_value())
		return player.value()->MakeAMove();

	return false;
}
#pragma endregion