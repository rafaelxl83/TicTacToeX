// This file contains the 'main' function. Program execution begins and ends there.

#include "../lib/MessageBus.h"
#include "../lib/Threads.h"

#include "WorkerCompanion.h"
#include "GamePlay.h"
#include "Players.h"

#include <map>

void FirstGame()
{
	Player* p1 = new Player(
		GetHashID(GenKey("PLAYER1")),
		"PLAYER1",
		Symbol(Symbol::AvailableSymbols::X),
		std::cin);

	Player* p2 = new Player(
		GetHashID(GenKey("PLAYER2")),
		"PLAYER2",
		Symbol(Symbol::AvailableSymbols::O),
		std::cin);

	Player* p3 = new Player(
		GetHashID(GenKey("PLAYER3")),
		"PLAYER3",
		Symbol(Symbol::AvailableSymbols::M),
		std::cin);

	Player* p4 = new Player(
		GetHashID(GenKey("PLAYER4")),
		"PLAYER4",
		Symbol(Symbol::AvailableSymbols::S),
		std::cin);

	Player* p5 = new Player(
		GetHashID(GenKey("PLAYER5")),
		"PLAYER5",
		Symbol(Symbol::AvailableSymbols::H),
		std::cin);

	Board* board = new Board(
		BoardSizes::FivePlayers, 
		GetHashID(GenKey("BOARD1")));

	p1->AddBoardId(board->GetID());
	p2->AddBoardId(board->GetID());
	p3->AddBoardId(board->GetID());
	p4->AddBoardId(board->GetID());
	p5->AddBoardId(board->GetID());

	Players* p = new Players(1);
	p->AddPlayer(p1);
	p->AddPlayer(p2);
	p->AddPlayer(p3);
	p->AddPlayer(p4);
	p->AddPlayer(p5);

	GamePlay* g = new GamePlay(1);
	g->AddBoard(board);

	Threads::GetInstance().Start();
	MessageBus::GetInstance();

	std::shared_ptr<Players> playersPtr(p);
	Threads::GetInstance().AddPlayerWork([playersPtr]()
		{
			playersPtr->Start();
		});

	std::shared_ptr<GamePlay> gamePlayPtr(g);
	Threads::GetInstance().AddBoardWork([&gamePlayPtr]()
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

	FirstGame();



	system("CLS");

	return 0;
}