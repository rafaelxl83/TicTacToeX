// This file contains the 'main' function. Program execution begins and ends there.

#include "../lib/MessageBus.h"
#include "../lib/Threads.h"

#include "WorkerCompanion.h"
#include "GamePlay.h"
#include "Players.h"
#include "NPCPlayer.h"

#include <map>

void CustomGame()
{
	int amountPlayers = 1;
	int boardSize = 3;
	
	std::cout << "Please, enter the type of the board [2-5]:" << std::endl;
	std::cout << "(2) Two players" << std::endl;
	std::cout << "(3) Three players" << std::endl;
	std::cout << "(4) Four players" << std::endl;
	std::cout << "(5) Five players" << std::endl;

	do
	{
		std::cin >> boardSize;
	} while (boardSize > 5 || boardSize < 2);

	std::cout << "How many humans will play? [0-" << boardSize << "]" << std::endl;
	do
	{
		std::cin >> amountPlayers;
	} while (boardSize < amountPlayers || amountPlayers < 0);

	BoardSizes bs;
	switch (boardSize)
	{
	case 3: bs = BoardSizes::ThreePlayers; break;
	case 4: bs = BoardSizes::FourPlayers; break;
	case 5: bs = BoardSizes::FivePlayers; break;
	default: bs = BoardSizes::TwoPlayers; break;
	}

	Board* board = new Board(
		bs, GetHashID(GenKey("BOARD1")));
	
	Players* p = new Players(1);

	int i = 1;
	for (; i <= amountPlayers; i++)
	{
		std::string pID = "PLAYER" + std::to_string(i);
		p->AddPlayer(
			new Player(
				GetHashID(GenKey(pID.c_str())),
				pID,
				Symbol(i),
				std::cin)
		);
		p->GetPlayer(i-1).value()->AddBoardId(board->GetID());
	}

	for (; i <= boardSize; i++)
	{
		std::string pID = "NPC" + std::to_string(i);
		p->AddPlayer(
			new NPCPlayer(
				GetHashID(GenKey(pID.c_str())),
				pID,
				Symbol(i))
		);
		p->GetPlayer(i-1).value()->AddBoardId(board->GetID());
		((NPCPlayer*)p->GetPlayer(i-1).value())->SetBoard(board);
	}

	GamePlay* g = new GamePlay(1);
	g->AddBoard(board);

	Threads::GetInstance().Start();
	MessageBus::GetInstance();

	std::shared_ptr<Players> playersPtr(p);
	Threads::GetInstance().AddPlayerWork([=]()
		{
			playersPtr->Start();
		});

	std::shared_ptr<GamePlay> gamePlayPtr(g);
	Threads::GetInstance().AddBoardWork([=]()
		{
			gamePlayPtr->Start();
		});

	Threads::GetInstance().Wait();
}

int main()
{
	/*std::map<unsigned int, std::list<unsigned int>>	boardsLink;
	boardsLink.insert(std::pair<unsigned int, std::list<unsigned int>>(123456, { 23456,23456,2345 }));
	boardsLink.insert(std::pair<unsigned int, std::list<unsigned int>>(6546, { 645678,3456,7654 }));

	for (std::pair<unsigned int, std::list<unsigned int>> p : boardsLink)
	{
		std::cout << p.first << " ";
		for (auto l : p.second)
		{
			std::cout << l << " ";
		}
		std::cout << std::endl;
	}*/

	//FirstGame();
	CustomGame();


	system("CLS");

	return 0;
}