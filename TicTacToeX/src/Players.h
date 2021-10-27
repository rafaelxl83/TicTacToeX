#pragma once
#include "GamePlayMessages.h"
#include "Player.h"

#include <mutex>

class Players
{
public:
										Players();
										~Players();

	virtual void						Start();

	virtual void						OnMessageStartOfGame(
											const MessageStartOfGame&	aMessage) noexcept;
	virtual void						OnMessageTurnChanged(
											const MessageTurnChanged&	aMessage) noexcept;
	virtual void						OnMessageSingleMove(
											const MessageSingleMove&	aMessage) noexcept;

	void								Done();

protected:
	virtual void						Initialize();

private:
	std::vector<Player*>				myPlayers;
	unsigned int						myPlayersLimitCount;
	bool								done = false;
};