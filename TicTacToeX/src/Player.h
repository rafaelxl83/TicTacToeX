#pragma once

#include <vector>

#include "Symbol.h"

class Player
{
public:
	enum class PlayerState
	{
		Idle = 0,
		Turn,
		Looser,
		Winner,
	};
										Player(
											std::string					aNickName);
										Player(
											Symbol						aSymbol);
										Player(
											std::string					aNickName,
											Symbol						aSymbol);
										Player(
											unsigned int				anId,
											std::string					aNickName,
											Symbol						aSymbol);
										Player(
											unsigned int				anId,
											std::string					aNickName,
											Symbol						aSymbol, 
											std::istream&				aStream);

	unsigned int						MakeAMove();
	
	void								SetSymbol(
											Symbol						aSymbol);

	void								SetState(
											PlayerState					aState);
	PlayerState							GetState() const;

	unsigned int						GetID();
	std::string							GetNickName();
	Symbol								GetPlayerSymbol();

	std::vector<unsigned int>			GetAllBoards();
	unsigned int						GetBoardId(
											int							index);
	int									GetBoardIndex(
											unsigned int				aBoardId);

	bool								AddBoardId(
											unsigned int				aBoardId);
	bool								RemBoard(
											int							index);
	bool								RemBoard(
											unsigned int				aBoardId);

	std::string							ToString() const;

private:
	PlayerState							myState = PlayerState::Idle;
	unsigned int						myId;
	Symbol								mySymbol;
	std::string							myNickName;
	std::vector<unsigned int>			myBoards;
	std::istream&						myStream;

};

#ifdef _DEBUG
#define PLAYER_DEFAULT_ID				static_cast<unsigned int>(GetHashID("PLAYER15102344370000000"))
#else
#define PLAYER_DEFAULT_ID				static_cast<unsigned int>(GetHashID(GenKey("PLAYER")))
#endif
#define NO_MARK							0