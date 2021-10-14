#pragma once
#include "Player.h"
#include "GamePlayMessages.h"

#include <mutex>

class GamePlay
{
public:
							GamePlay();
							GamePlay(
								std::list<Player>			allPlayers);
							~GamePlay();

	void					SetPlayers(
								std::vector<Player>			allPlayers);

	virtual void			Start();

	virtual void			OnMessageEndOfGame(
								const MessageEndOfGame&		aMessage) noexcept;
	virtual void			OnMessageStartOfGame(
								const MessageStartOfGame&	aMessage) noexcept;
	virtual void			OnMessageTurnChanged(
								const MessageTurnChanged&	aMessage) noexcept;
	virtual void			OnMessageSingleMove(
								const MessageSingleMove&	aMessage) noexcept;
	virtual void			OnMessageBlockMove(
								const MessageBlockMove&		aMessage) noexcept;
	virtual void			OnMessageScorePoints(
								const MessageScorePoints&	aMessage) noexcept;
	
	void					ShutDown();

protected:
	virtual void			Initialize();
	virtual void			Turn();

private:
	std::vector<Player*>	myPlayers;
	unsigned int			myPlayersCount;
	bool					shutDown = false;

};