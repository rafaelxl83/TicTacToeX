#pragma once
#include "Player.h"
#include "GamePlayMessages.h"

#include <mutex>
#include <map>

class GamePlay
{
public:
							GamePlay();
							GamePlay(
								std::list<Player>			allPlayers);
							~GamePlay();

	virtual void			OnMessageShutdown(
								const MessageShutdown&		aMessage) noexcept;
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

	virtual void			Start();
	virtual void			ShutDown();

	void					AddPlayer(
								Player*						aPlayer);
	void					SetPlayers(
								std::vector<Player>			allPlayers);

	std::string				PrintBoard();

protected:
	virtual void			Initialize();
	virtual void			Turn();

private:
	std::vector<Player*>	myPlayers;
	unsigned int			myPlayersCount;
	short					myTurnCounter;
	bool					shutDown = false;
	std::map<short, Point>	boardMap;

};