#pragma once
#include "Player.h"
#include "Board.h"

#include <iostream>
#include <functional>


class NPCPlayer : public Player
{
public:
									NPCPlayer(
										unsigned int				anId,
										std::string					aNickName,
										Symbol						aSymbol);

	unsigned int					MakeAMove() override;

	void							SetBoard(
										std::shared_ptr<Board>		aBoard);

private:
	int								EvaluateBoard();

	int								CheckSection(
										Point						aPoint,
										std::function<int (int)>	eValuation);
	int								CheckLine(
										Point						aPos1, 
										Point						aPos2, 
										Point						aPos3,
										Point						aPoint,
										std::shared_ptr<Row[]>		aSection,
										std::function<int (int)>	eValuation);
	int								Evaluate(
										int							aValue);

	std::shared_ptr<Board>			theBoard;
	std::stringbuf					buffer;
};

