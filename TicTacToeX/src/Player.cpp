#include "Player.h"

#include "../lib/Utils.h"

Player::Player(
	std::string					aNickName)
	: Player(
		GetHashID(GenKey(aNickName.c_str())),
		aNickName,
		Symbol::AvailableSymbols::X, 
		std::cin)
{
}
Player::Player(
	Symbol						aSymbol)
	: Player(
		PLAYER_DEFAULT_ID,
		GenKey("PLAYER"),
		aSymbol, 
		std::cin)
{
}
Player::Player(
	std::string					aNickName,
	Symbol						aSymbol)
	: Player(
		GetHashID(GenKey(aNickName.c_str())),
		aNickName,
		aSymbol, 
		std::cin)
{
}
Player::Player(
	unsigned int				anId,
	std::string					aNickName,
	Symbol						aSymbol)
	: Player(
		anId,
		aNickName,
		aSymbol,
		std::cin)
{
}
Player::Player(
	unsigned int				anId,
	std::string					aNickName,
	Symbol						aSymbol,
	std::istream&				aStream)
	: myId(anId)
	, myNickName(aNickName)
	, mySymbol(aSymbol)
	, myStream(aStream)
{
	Log("[Player]", myId, "Created", ToString());
}

unsigned int
Player::MakeAMove()
{
	unsigned int mark = NO_MARK;
	if (myState == PlayerState::Turn)
	{
		myStream >> mark;
	}

	return mark;
}

void
Player::SetSymbol(
	Symbol						aSymbol)
{
	mySymbol = aSymbol;
}

void
Player::SetState(
	PlayerState					aState)
{
	myState = aState;
}

Player::PlayerState
Player::GetState() const
{
	return myState;
}

unsigned int
Player::GetID()
{
	return myId;
}
std::string
Player::GetNickName()
{
	return myNickName;
}
Symbol
Player::GetPlayerSymbol()
{
	return mySymbol;
}

std::vector<unsigned int>
Player::GetAllBoards()
{
	return myBoards;
}

unsigned int
Player::GetBoardId(
	int							index)
{
	return myBoards[index];
}

int
Player::GetBoardIndex(
	unsigned int				aBoardId)
{
	auto itr = std::find(myBoards.begin(), myBoards.end(), aBoardId);

	if(itr != myBoards.end())
		return std::distance(myBoards.begin(), itr);
	return -1;
}

bool
Player::AddBoardId(
	unsigned int				aBoardId)
{
	if (GetBoardIndex(aBoardId) >= 0)
		return false;

	myBoards.push_back(aBoardId);
	return true;
}

bool
Player::RemBoard(
	int							index)
{
	size_t s = myBoards.size();
	if (0 < index && index < s)
		myBoards.erase(myBoards.begin() + index);

	return myBoards.size() < s;
}
bool
Player::RemBoard(
	unsigned int				aBoardId)
{
	int index = GetBoardIndex(aBoardId);

	if (index >= 0)
		return RemBoard(index);

	return false;
}

std::string
Player::ToString() const
{
	std::stringbuf buffer;
	std::ostream os(&buffer);

	os << "Player [" << myNickName << "]\n"
	   << "ID     [" << myId	   << "]\n"
	   << "Symbol [" << mySymbol   << "]\n"
	   << "Boards [";

	std::copy(
		myBoards.begin(),
		myBoards.end(),
		std::ostream_iterator<unsigned int>(os, ", "));
	os << "]";

	return buffer.str();
}