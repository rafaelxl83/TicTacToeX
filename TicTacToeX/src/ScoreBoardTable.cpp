#include "ScoreBoardTable.h"
#include "ExceptionHelper.h"

#include <chrono>
#include <ranges>

ScoreBoardTable::ScoreBoardTable()
	:ScoreBoardTable(SCB_DEFAULT_ID)
{
}
ScoreBoardTable::ScoreBoardTable(
	unsigned int				anId)
	: myId(anId)
{
	Initialize();
}
ScoreBoardTable::~ScoreBoardTable()
{

}

void
ScoreBoardTable::ScorePoints(
	unsigned int				aBoardID,
	unsigned int				aPlayerID,
	double						thePoints,
	char						anIco)
{
	if (aPlayerID <= 0)
	{
		throw std::invalid_argument(
			"Invalid argument: not a valid player ID.");
	}

	try
	{
		std::optional<ScoreRow*> scb = GetPlayersScore(aPlayerID, aBoardID);
		if (scb.has_value())
		{
			scb.value()->points += thePoints;
		}
		else
		{
			scoredPoints.push_back(ScoreRow(
				aBoardID,
				aPlayerID,
				thePoints,
				anIco
			));
		}

		Log("[ScoreBoardTable]", myId, "ScorePoints", "points scored: [", aBoardID, ";", aPlayerID, "]");
	}
	catch (...)
	{
		Log("[ScoreBoardTable]", myId, "ScorePoints", "Exception:", ExceptionHelper::what());
	}
}

unsigned int
ScoreBoardTable::GetID()
{
	return myId;
}

std::vector<ScoreBoardTable::ScoreRow>
ScoreBoardTable::GetAllScoredPoints()
{
	return scoredPoints;
}
std::optional<ScoreBoardTable::ScoreRow*>
ScoreBoardTable::GetPlayersScore(
	int							anIndex)
{
	return std::make_optional(&scoredPoints[anIndex]);
}
std::optional<ScoreBoardTable::ScoreRow*>
ScoreBoardTable::GetPlayersScore(
	unsigned int			aPlayerId, 
	unsigned int			aBoardId)
{
	try
	{
		// linear complexity O(n)
		auto scb = std::ranges::find_if(
			scoredPoints,
			[aPlayerId, aBoardId](ScoreRow p) 
			{ 
				return (p.playerID == aPlayerId && p.boardID == aBoardId);
			});

		if (scb != scoredPoints.end())
			return std::make_optional(&*scb);
	}
	catch (std::out_of_range& ex)
	{
		Log("[ScoreBoardTable]", myId, "GetPlayersScore", "Exception", ex.what());
	}
	catch (...)
	{
		Log("[ScoreBoardTable]", myId, "GetPlayersScore", "Unhandled Exception:", ExceptionHelper::what());
	}

	return std::nullopt;
}

std::string
ScoreBoardTable::PrintScoreboard(
	unsigned int				aBoardId)
{
	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::vector<ScoreRow> scb;

	try
	{
		// linear complexity O(n)
		std::ranges::copy_if(
			scoredPoints,
			std::back_inserter(scb),
			[aBoardId](ScoreRow& s) { return (s.boardID == aBoardId); });

		std::ranges::sort(scb, [](ScoreRow& s1, ScoreRow& s2) { return s1.points > s2.points; });

		std::copy(scb.begin(), scb.end(),
			std::ostream_iterator<ScoreRow>(os, "\n"));
	}
	catch (std::out_of_range& ex)
	{
		Log("[EventTable]", myId, "GetPlayersEvents", "Exception", ex.what());
	}
	catch (...)
	{
		Log("[EventTable]", myId, "GetPlayersEvents", "Unhandled Exception:", ExceptionHelper::what());
	}

	return "Scoreboard - "
		+ std::to_string(aBoardId)
		+ ":\n"
		+ buffer.str();
}

std::string
ScoreBoardTable::ToString() const
{
	std::stringbuf buffer;
	std::ostream os(&buffer);
	std::copy(scoredPoints.begin(),
		scoredPoints.end(),
		std::ostream_iterator<ScoreRow>(os, "\n"));

	return "ScoreBoardTable - State: "
		+ std::to_string(myId)
		+ "\n"
		+ buffer.str();
}

void
ScoreBoardTable::Initialize()
{
	scoredPoints = std::vector<ScoreRow>();
}
