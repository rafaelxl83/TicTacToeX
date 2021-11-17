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
		if (aMessage.theWinner != (int)Symbol::AvailableSymbols::empty)
		{
			// print the winner and the players score
		}
		else
		{
			// print the players score
		}
		Done();
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

		std::optional<Player*> p = GetPlayer(0);
		if (!p.has_value() || !p.value()->GetBoardId(0).has_value())
		{
			Log("[Players]", "OnMessageStartOfGame",
				"FATAL", "There are no player or board to play!");
			return;
		}

		SEND_TO_GAMEPLAY(MessageStartOfGame(
			aMessage.callerId,
			p.value()->GetID(),
			p.value()->GetBoardId(0).value()));

		// call the turn of events
		SEND_TO_PLAYERS(MessageTurnChanged(
			aMessage.callerId, 
			p.value()->GetID(),
			p.value()->GetBoardId(0).value(),
			(int)TurnActions::Retake));
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

		Turn((TurnActions)aMessage.turn);

		std::optional<Player*> player = GetPlayer(playerTurn);
		if (!player.has_value() || !player.value()->GetBoardId(0).has_value())
		{
			Log("[Players]", "MessageTurnChanged",
				"FATAL", "There are no player or board to play!");
			return;
		}

		int symbol = (int)player.value()->
			GetPlayerSymbol().
			GetProperty().symbol;

		// get the first available player to make a movement
		// all others must be with the idle state
		// with the correct player listed must send the message
		// to perform a movement
		if(player.value()->GetState() == Player::PlayerState::Turn)
			SEND_TO_PLAYERS(MessageSingleMove(
				aMessage.callerId,
				player.value()->GetID(),
				player.value()->GetBoardId(0).value(), 
				0, symbol
				));

		// call the turn of events
		// just the board can confirm if its
		// time to change the player turn
		SEND_TO_PLAYERS(MessageTurnChanged(
			aMessage.callerId,
			player.value()->GetID(),
			player.value()->GetBoardId(0).value(),
			(int)TurnActions::None));
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
			if (mark == NO_MARK) return;

			player.value()->SetState(Player::PlayerState::Idle);

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
	if (myPlayers.size() == 0)
	{
		Log("[Players]", "Start",
			"FATAL", "There are no players!");
		return;
	}

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
	myPlayersLimitCount = myPlayers.size() - 1;

	for (Player* p : myPlayers)
		p->SetState(Player::PlayerState::Idle);
	myPlayers[0]->SetState(Player::PlayerState::Turn);
}

void
Players::Turn(
	TurnActions						anAction)
{
	int lastTurn = playerTurn;
	switch (anAction)
	{
		case TurnActions::Next:
			if (playerTurn == myPlayersLimitCount)
				playerTurn = 0;
			else
				playerTurn++;

			myPlayers[lastTurn]->SetState(Player::PlayerState::Idle);
		case TurnActions::Retake:
			myPlayers[playerTurn]->SetState(Player::PlayerState::Turn);
			break;
		case TurnActions::None:
		default: 
			break;
	}
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

	return NO_MARK;
}
unsigned int
Players::GetInput(
	unsigned int				aPlayerId)
{
	std::optional<Player*> player = GetPlayer(aPlayerId);
	if (player.has_value())
		return player.value()->MakeAMove();

	return NO_MARK;
}
#pragma endregion