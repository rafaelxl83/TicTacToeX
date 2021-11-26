#pragma once
#include "Board.h"
#include "EventTable.h"
#include "ScoreBoardTable.h"
#include "GamePlayMessages.h"

#include <mutex>
#include <optional>

class GamePlay
{
public:
									GamePlay();
									GamePlay(
										const unsigned int			anId);
									GamePlay(
										std::vector<Board*>			allBoards);
									GamePlay(
										const unsigned int			anId,
										std::vector<Board*>			allBoards);
									~GamePlay();

	virtual void					OnMessageShutdown(
										const MessageShutdown&		aMessage) noexcept;
	virtual void					OnMessageEndOfGame(
										const MessageEndOfGame&		aMessage) noexcept;
	virtual void					OnMessageStartOfGame(
										const MessageStartOfGame&	aMessage) noexcept;
	virtual void					OnMessageTurnChanged(
										const MessageTurnChanged&	aMessage) noexcept;
	virtual void					OnMessageSingleMove(
										const MessageSingleMove&	aMessage) noexcept;
	virtual void					OnMessageBlockMove(
										const MessageBlockMove&		aMessage) noexcept;
	virtual void					OnMessageScorePoints(
										const MessageScorePoints&	aMessage) noexcept;

	virtual void					Start();
	virtual void					ShutDown();

	void							AddBoard(
										Board*						aBoard);
	void							AddBoards(
										std::vector<Board*>			allBoards);

	std::optional<Board*>			GetBoard(
										int							anIndex);
	std::optional<Board*>			GetBoard(
										unsigned int				aBoardId);
	std::vector<Board*>				GetAllBoards();

	std::string						PrintBoard(
										int							anIndex);
	std::string						PrintBoard(
										unsigned int				aBoardId);

	std::optional<EventTable*>		GetEvtTable(
										int							anIndex);
	std::optional<EventTable*>		GetEvtTable(
										unsigned int				aTableId);
	std::vector<EventTable*>		GetAllEvtTables();

	std::optional<ScoreBoardTable*>	GetScoreTable(
										int							anIndex);
	std::optional<ScoreBoardTable*>	GetScoreTable(
										unsigned int				aTableId);
	std::vector<ScoreBoardTable*>	GetAllScoreTables();

#ifndef _DEBUG
protected:
#endif // !_DEBUG
	bool						SetMark(
		int							anIndex,
		short						aPos,
		Symbol						aSymbol);
	bool						SetMark(
		unsigned int				aBoardId,
		short						aPos,
		Symbol						aSymbol);

	Symbol						HasWinner(
		Board& aBoard);
	int							CheckSection(
		Board& aBoard,
		Point						aPoint);
	int							Evaluate(
		int							aValue);

private:
	virtual void				Initialize();
	virtual void				Turn();

	unsigned int					myId;
	std::vector<Board*>				myBoards;
	std::vector<EventTable*>		myEvtTables;
	std::vector<ScoreBoardTable*>	myScoreTables;
	unsigned int					myPlayersCount;
	short							myTurnCounter;
	Symbol							theWinner;
	bool							shutDown = false;

};

#define GPLAY_DEFAULT_ID				static_cast<unsigned int>(GetHashID("PLAY15102344370000000"))