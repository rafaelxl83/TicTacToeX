#pragma once
#include "GamePlayMessages.h"
#include "Player.h"

#include <mutex>
#include <map>

class Players
{
public:
							Players();
							Players(
								const unsigned int			anId);
							Players(
								std::vector<Player*>		allPlayers);
							Players(
								const unsigned int			anId,
								std::vector<Player*>		allPlayers);
							~Players();

	virtual void			Start();

	virtual void			OnMessageEndOfGame(
								const MessageEndOfGame&		aMessage) noexcept;
	virtual void			OnMessageStartOfGame(
								const MessageStartOfGame&	aMessage) noexcept;
	virtual void			OnMessageTurnChanged(
								const MessageTurnChanged&	aMessage) noexcept;
	virtual void			OnMessageSingleMove(
								const MessageSingleMove&	aMessage) noexcept;

	virtual void			Done();

	void					AddPlayer(
								Player*						aPlayer);
	void					AddPlayers(
								std::vector<Player*>		allPlayers);

	std::optional<Player*>	GetPlayer(
								int							anIndex);
	std::optional<Player*>	GetPlayer(
								unsigned int				aPlayerId);
	std::vector<Player*>	GetAllPlayer();

private:
	virtual void			Initialize();
	virtual void			Turn(
								bool						nextPlayer);

	unsigned int			GetInput(
								int							anIndex);
	unsigned int			GetInput(
								unsigned int				aPlayerId);

	unsigned int							myId;
	std::vector<Player*>					myPlayers;
	unsigned int							myPlayersLimitCount;
	int										playerTurn = 0;
	bool									done = false;
};

#define PLAYERS_DEFAULT_ID					static_cast<unsigned int>(GetHashID("PLAY15102344370000000"))