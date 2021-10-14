#include "Threads.h"

#include <chrono>

using namespace std::literals;

// Shrink this number to make the system go faster.
static constexpr auto locThreadWaitTime = 50ms;

void
Threads::Worker(
	WorkerData&				aData)
{
	std::vector<std::function<void()>> work;

	while(true)
	{
		{
			std::unique_lock<std::mutex> lock(aData.myMutex);
			aData.myCv.wait_for(lock, locThreadWaitTime);

			work.swap(aData.myWork);
		}

		for(auto& wrk : work)
		{
			wrk();
		}

		work.clear();
	}
}

void
Threads::Start()
{
	myGamePlayData.myThread = std::thread(&Threads::Worker, std::ref(myGamePlayData));
	myPlayersData.myThread = std::thread(&Threads::Worker, std::ref(myPlayersData));
}

void
Threads::Wait()
{
	myGamePlayData.myThread.join();
	myPlayersData.myThread.join();
}

void
Threads::AddWork(
	std::function<void()>	aWork,
	WorkerData&				aWorker)
{
	{
		std::lock_guard<std::mutex> lock(aWorker.myMutex);
		aWorker.myWork.emplace_back(std::move(aWork));
	}

	aWorker.myCv.notify_all();
}

void
Threads::AddBoardWork(
	std::function<void()>	aWork)
{
	AddWork(std::move(aWork), myGamePlayData);
}

void
Threads::AddPlayerWork(
	std::function<void()>	aWork)
{
	AddWork(std::move(aWork), myPlayersData);
}
