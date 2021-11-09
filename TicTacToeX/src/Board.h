#pragma once

#include "Symbol.h"
#include "../lib/Utils.h"

#include <map>
#include <vector>
#include <optional>

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
										Board(							) = delete;
										Board(
											Board&&						) = default;
										Board(
											const Board&				) = delete;
										Board(
											unsigned int				anId);
										Board(
											BoardSizes					theSize,
											unsigned int				anId);
										~Board() = default;

	short								GetSize();
	unsigned int						GetID();
	std::optional<Point>				GetPoint(
											short						aPos);
	std::weak_ptr<Row[]>				GetSector(
											Point						aPos);
	std::vector<Point>					GetMarkedPositions();

	bool								SetMark(
											Point						aPos,
											Symbol						aSymbol);
	bool								SetMark(
											short						aPos,
											Symbol						aSymbol);

	bool								IsFull();

	void								PrintBoard(
											std::ostream&				aStream);

	std::string							ToString() const;

	/*Board&								operator= (
											const Board&				) = delete;
	Board&								operator= (
											Board&						) = default;*/
protected:
	void								Initialize();

private:
	unsigned int						myId;
	short								mySize;
	short								realSize;
	short								cellsAmount;

	std::shared_ptr<Row[]>				theSection;
	std::unique_ptr<Row[]>				theBoard;
	std::map<short, Point>				boardMap;

	std::vector<Point>					markedCells;
};

#ifdef _DEBUG
#define BOARD_DEFAULT_ID				static_cast<unsigned int>(GetHashID("BOARD15102344370000000"))
#else
#define BOARD_DEFAULT_ID				static_cast<unsigned int>(GetHashID(GenKey("BOARD")))
#endif