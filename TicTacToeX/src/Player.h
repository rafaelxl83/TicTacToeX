#pragma once

#include "Board.h"

struct PlayerProperties
{
	Symbol	aSymbol;
	int		theVictoryPoints;
};

class Player
{
public:
										Player(
											unsigned int				anId,
											std::string					aNickName,
											PlayerProperties			theProperties,
											std::shared_ptr<Board[]>	theBoards);

	unsigned int						GetID();
	bool								MakeMove(
											unsigned short				aPosition);

private:
	unsigned int						myID;
	std::string							myNickName;
	PlayerProperties					myProperties;
	std::shared_ptr<Board[]>			myBoards;

};

#define PLAYER_DEFAULT_ID			static_cast<unsigned int>(GetHashID("PLAYER15102344370000000"))