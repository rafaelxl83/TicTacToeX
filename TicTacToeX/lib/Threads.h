#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class Threads
{
public:
	static Threads&							GetInstance()
	{
		static Threads instance;
		return instance;
	}

	void									Start();
	void									AddBoardWork(
												std::function<void()>	aWork);
	void									AddPlayerWork(
												std::function<void()>	aWork);

	void									Wait();

private:

											Threads() = default;

	struct WorkerData
	{
		std::thread							myThread;
		std::vector<std::function<void()>>	myWork;
		std::condition_variable				myCv;
		std::mutex							myMutex;
	};

	static void								Worker(
												WorkerData& aData);

	static void								AddWork(
												std::function<void()>	aWork,
												WorkerData&				aWorker);
	
	WorkerData								myGamePlayData;
	WorkerData								myPlayersData;
};
