#pragma once
#include <list>
#include <vector>

#include "../lib/Utils.h"
#include "Symbol.h"

class EventTable
{
public:
	struct EventRow
	{
		double			timestamp = 0;
		unsigned int	playerID = 0;
		char			playerSymbol = 'X';
		Point			position;

		friend std::ostream& operator<<(
			std::ostream& os,
			const EventRow& obj)
		{
			return os << "(" <<
				obj.timestamp << " - " <<
				obj.playerID << " [" <<
				obj.position.x << "," <<
				obj.position.y << "])";
		}

		friend std::istream& operator<<(
			std::istream& is,
			const EventRow& obj)
		{
			return is;
		}
	};

										EventTable();
										EventTable(
											unsigned int				anId);
										~EventTable();

	void								RegisterEvent(
											unsigned int				aPlayerID,
											Symbol						aSymbol,
											Point						aPosition);

	unsigned int						GetID();

	std::vector<EventRow>				GetAllEvents();
	std::vector<EventRow>				GetPlayersEvents(
											unsigned int				aPlayerID);
	std::vector<EventRow>				GetPlayersEvents(
											std::list<unsigned int>		thePlayersID);

	std::string							ToString() const;

protected:
	void								Initialize();

private:
	unsigned int						myId;
	std::vector<EventRow>				allPlayedEvents;

};

#ifdef _DEBUG
#define EVT_DEFAULT_ID					static_cast<unsigned int>(GetHashID("EVTTABLE15102344370000000"))
#else
#define EVT_DEFAULT_ID					static_cast<unsigned int>(GetHashID(GenKey("EVTTABLE")))
#endif
