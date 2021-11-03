#pragma once
#include "Board.h"
#include "GamePlayMessages.h"

#include <mutex>


class GamePlay
{
public:
							GamePlay();
							GamePlay(
								std::list<Board>			allBoards);
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

	void					AddBoard(
								Board*						aBoard);
	void					SetBoards(
								std::vector<Board*>			allBoards);

	Board&					GetBoard(
								int							anIndex);
	Board&					GetBoard(
								unsigned int				aBoardId);

	std::string				PrintBoard();

//protected:
	virtual void			Initialize();
	virtual void			Turn();
	Symbol					HasWinner(
								Board&						aBoard);
	int						CheckSection(
								Board&						aBoard,
								Point						aPoint);
	int						Evaluate(
								int							aValue);

private:
	std::vector<Board*>		myBoards;
	unsigned int			myPlayersCount;
	short					myTurnCounter;
	bool					shutDown = false;

};