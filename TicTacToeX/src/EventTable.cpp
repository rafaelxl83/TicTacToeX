#include <chrono>
#include <ranges>


#include "Board.h"
#include "ExceptionHelper.h"

EventTable::EventTable()
	: EventTable(EVT_DEFAULT_ID)
{
}

EventTable::EventTable(
	unsigned int				anId)
	: myId(anId)
{
	Initialize();
}

EventTable::~EventTable()
{
	allPlayedEvents.~vector();
}

void
EventTable::Initialize()
{
	allPlayedEvents = std::vector<EventRow>();
}

void
EventTable::RegisterEvent(
	unsigned int				aPlayerID,
	Symbol						aSymbol,
	Point						aPosition)
{
	if (aPlayerID <= 0)
	{
		throw std::invalid_argument(
			"Invalid argument: not a valid player ID.");
	}
	if (aSymbol.GetProperty().value < 0) {
		throw std::invalid_argument(
			"Invalid argument: it isn't a valid symbol.");
	}

	try
	{
		double timestamp = std::chrono::system_clock::now().time_since_epoch().count();
		allPlayedEvents.push_back(EventRow(
			timestamp, 
			aPlayerID, 
			aSymbol.GetProperty().ico, 
			aPosition
		));

		Log("[EventTable]", myId, "RegisterEvent", "registered event: [", timestamp ,";", aPlayerID, "]");
	}
	catch (...)
	{
		Log("[EventTable]", myId, "RegisterEvent", "Exception:", ExceptionHelper::what());
	}

}

unsigned int
EventTable::GetID()
{
	return myId;
}

std::vector<EventRow>
EventTable::GetAllEvents()
{
	std::vector<EventRow> evt;
	//int size = allPlayedEvents.size();
	//EventRow* evt = new EventRow[size];
	//EventRow* evt = &allPlayedEvents[0];
	//EventRow* evt = allPlayedEvents.data();

	std::ranges::copy(allPlayedEvents.begin(), allPlayedEvents.end(), std::back_inserter(evt));
	return evt;
}

std::vector<EventRow>
EventTable::GetPlayersEvents(
	unsigned int				aPlayerID)
{
	//std::vector<EventRow> evt;

	//try
	//{
	//	// linear complexity O(n)
	//	std::ranges::copy_if(
	//		allPlayedEvents, 
	//		std::back_inserter(evt),
	//		[aPlayerID](EventRow& e) { return (e.playerID == aPlayerID); }
	//	);
	//}
	//catch (std::out_of_range& ex)
	//{
	//	Log("[EventTable]", myId, "GetPlayerEvents", "Exception", ex.what());
	//}
	//catch (...)
	//{
	//	Log("[EventTable]", myId, "GetPlayerEvents", "Unhandled Exception:", ExceptionHelper::what());
	//}

	return GetPlayersEvents( std::list<unsigned int>{ aPlayerID } );
}

std::vector<EventRow>
EventTable::GetPlayersEvents(
	std::list<unsigned int>		thePlayersID)
{
	std::vector<EventRow> evt;
	auto pred = [thePlayersID](EventRow& e) 
	{ 
		return (std::find(thePlayersID.begin(), thePlayersID.end(), e.playerID) != thePlayersID.end()); 
	};

	try 
	{
		// linear complexity O(n)
		std::ranges::copy_if(
			allPlayedEvents, 
			std::back_inserter(evt), 
			pred);
	}
	catch (std::out_of_range& ex)
	{
		Log("[EventTable]", myId, "GetPlayersEvents", "Exception", ex.what());
	}
	catch (...)
	{
		Log("[EventTable]", myId, "GetPlayersEvents", "Unhandled Exception:", ExceptionHelper::what());
	}

	return evt;
}

std::string
EventTable::ToString() const
{
	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::copy(allPlayedEvents.begin(),
		allPlayedEvents.end(),
		std::ostream_iterator<EventRow>(os, "\n"));

	return "EventTable - State: "
		+ std::to_string(myId)
		+ "\n["
		+ buffer.str()
		+ ']';
}