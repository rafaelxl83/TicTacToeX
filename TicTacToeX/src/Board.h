#pragma once

#include "../lib/Utils.h"
#include "EventTable.h"

#define VERIFICATION_ADJUSTMENT 2

enum class BoardSizes : short
{
	TwoPlayers= 3,
	ThreePlayers,
	FourPlayers,
	FivePlayers,
};

typedef std::vector<Symbol> Row;

class Board
{
public:
										Board(
											unsigned int				anEvtTableId);
										Board(
											unsigned int				anId,
											unsigned int				anEvtTableId);
										Board(
											unsigned int				anId,
											BoardSizes					theSize,
											unsigned int				anEvtTableId);
										~Board();

	unsigned int						GetID();
	unsigned int						GetEvtTableID();

	bool								SetMark(
											Point						aPos,
											Symbol						aSymbol);

	std::weak_ptr<Row[]>				GetSector(
											Point						aPos);

	std::vector<Point>					GetMarkedPositions();

	std::string							ToString() const;
	
protected:
	void Initialize();

private:
	union Union
	{
		BoardSizes	s;
		short		i;
	};

	unsigned int						myId;
	short								mySize;
	short								realSize;
	unsigned int						myEvtTableId;

	std::shared_ptr<Row[]>				theSection;
	std::unique_ptr<Row[]>				theBoard;

	std::unique_ptr<bool[]>				refHorizontal;
	std::unique_ptr<bool[]>				refVertical;

	std::vector<Point>					markedCells;
};

#define BOARD_DEFAULT_ID				static_cast<unsigned int>(GetHashID("BOARD15102344370000000"))