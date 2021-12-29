#include "WorkerCompanion.h"

#include <chrono>

#pragma region "Stoppagle class methods"
Stoppable::Stoppable()
	: futureObj(
		exitSignal.get_future())
{
}

Stoppable::Stoppable(
	Stoppable &&							obj)
	: exitSignal(
		std::move(obj.exitSignal))
	, futureObj(std::move(obj.futureObj))
{
}

Stoppable &
Stoppable::operator=(
	Stoppable &&							obj)
{
	exitSignal = std::move(obj.exitSignal);
	futureObj = std::move(obj.futureObj);
	return *this;
}

void
Stoppable::operator()()
{
	run();
}

//Checks if thread is requested to stop
bool 
Stoppable::stopRequested()
{
	// checks if value in future object is available
	if (futureObj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
		return false;
	return true;
}

void 
Stoppable::stop()
{
	exitSignal.set_value();
}
#pragma endregion

WorkerCompanion::WorkerCompanion(
	std::shared_ptr<GamePlay>				gameplay,
	std::shared_ptr<Players>				players,
	Threads&								threads)
	: myGamePlay(gameplay)
	, myPlayers(players)
	, theThreads(threads)
{
}

void WorkerCompanion::run()
{
	// Check if thread is requested to stop ?
	while (!stopRequested())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		m.lock();
		if (myPlayers->IsDone() && myGamePlay->Completed())
		{
			theThreads.Interrupt();
			this->stop();
		}
		m.unlock();
	}
}