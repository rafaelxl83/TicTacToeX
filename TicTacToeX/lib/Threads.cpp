#include "Threads.h"

#include <chrono>

using namespace std::literals;

// Shrink this number to make the system go faster.
static constexpr auto locThreadWaitTime = 500ms;

void
Threads::Worker(
	WorkerData&				aData)
{
	std::vector<std::function<void()>> work;

	try
	{
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

			if (boost::this_thread::interruption_requested())
				break;
		}
	}
	catch (boost::thread_interrupted&)
	{
		return;
	}
}

void
Threads::Start()
{
	Reset();

	myGamePlayData.myThread = boost::thread(&Threads::Worker, std::ref(myGamePlayData));
	myPlayersData.myThread = boost::thread(&Threads::Worker, std::ref(myPlayersData));
}

void
Threads::Wait()
{
	myGamePlayData.myThread.join();
	myPlayersData.myThread.join();
}

void
Threads::Interrupt()
{
	myGamePlayData.myThread.interrupt();
	myPlayersData.myThread.interrupt();
}

void
Threads::Reset()
{
	myGamePlayData.myWork.clear();
	myPlayersData.myWork.clear();

	myPlayersData.myWork.shrink_to_fit();
	myPlayersData.myWork.shrink_to_fit();
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
