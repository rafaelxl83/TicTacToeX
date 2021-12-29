#pragma once
#include <future>

#include "GamePlay.h"
#include "Players.h"
#include "..\lib\Threads.h"

class Stoppable
{
public:
								Stoppable();
								Stoppable(
									Stoppable &&					obj);

	Stoppable &					operator=(
									Stoppable &&					obj);

	virtual void				run() = 0;

	void						operator()();

	bool						stopRequested();

	void						stop();

protected:
	std::mutex					m;

private:
	std::promise<void>			exitSignal;
	std::future<void>			futureObj;
};

class WorkerCompanion : public Stoppable
{
public:
								WorkerCompanion() = default;
								WorkerCompanion(
									std::shared_ptr<GamePlay>		gameplay,
									std::shared_ptr<Players>		palyers,
									Threads&						threads);

	void						run();

private:
	std::shared_ptr<GamePlay>	myGamePlay;
	std::shared_ptr<Players>	myPlayers;
	Threads&					theThreads;
};