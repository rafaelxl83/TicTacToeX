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
											std::shared_ptr<EventTable>	anEvtTable);
										Board(
											unsigned int				anId,
											std::shared_ptr<EventTable>	anEvtTable);
										Board(
											unsigned int				anId,
											BoardSizes					theSize,
											std::shared_ptr<EventTable>	anEvtTable);
										~Board();

	unsigned int						GetID();
	std::shared_ptr<EventTable>			GetEvtTable();
	bool								SetMark(
											Point						aPos,
											Symbol						aSymbol);

	std::weak_ptr<Row[]>				GetSector(
											Point						aPos);

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
	std::shared_ptr<EventTable>			myEvtTable;

	std::shared_ptr<Row[]>				theSection;
	std::unique_ptr<Row[]>				theBoard;

	std::unique_ptr<bool[]>				refHorizontal;
	std::unique_ptr<bool[]>				refVertical;

};

#define BOARD_DEFAULT_ID			static_cast<unsigned int>(GetHashID("BOARD15102344370000000"))