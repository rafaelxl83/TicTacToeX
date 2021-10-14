#pragma once
#include <list>
#include <vector>

#include "../lib/Utils.h"
#include "Symbol.h"

struct EventRow
{
	double			timestamp = 0;
	unsigned int	playerID = 0;
	char			playerSymbol = 'X';
	Point			position;

	friend std::ostream& operator<<(std::ostream& os, const EventRow& obj) 
	{
		return os << "(" << 
			obj.timestamp << " - " << 
			obj.playerID << " [" << 
			obj.position.x << "," << 
			obj.position.y << "])";
	}

	friend std::istream& operator<<(std::istream& is, const EventRow& obj) 
	{
		return is;
	}
};

class EventTable
{
public:
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
	std::vector<EventRow>				GetPlayerEvents(
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

#define EVT_DEFAULT_ID			static_cast<unsigned int>(GetHashID("EVTTABLE15102344370000000"))
