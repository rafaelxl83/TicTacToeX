#pragma once
#include <list>
#include <vector>
#include <optional>

#include "../lib/Utils.h"

class ScoreBoardTable
{
public:
	struct ScoreRow
	{
		unsigned int	boardID = 0;
		unsigned int	playerID = 0;
		double			points = 0;
		char			ico = ' ';

		friend std::ostream& operator<<(
			std::ostream& os,
			const ScoreRow& obj)
		{
			return os			<< "[" <<
				obj.boardID		<< " - " <<
				obj.playerID	<< " (" <<
				obj.ico			<< "): " <<
				obj.points		<< "]";
		}

		friend std::istream& operator<<(
			std::istream& is,
			const ScoreRow& obj)
		{
			return is;
		}
	};

										ScoreBoardTable();
										ScoreBoardTable(
											unsigned int				anId);
										~ScoreBoardTable();

	void								ScorePoints(
											unsigned int				aBoardID,
											unsigned int				aPlayerID,
											double						thePoints,
											char						anIco);

	unsigned int						GetID();

	std::vector<ScoreRow>				GetAllScoredPoints();
	std::optional<ScoreRow*>			GetPlayersScore(
											int							anIndex);
	std::optional<ScoreRow*>			GetPlayersScore(
											unsigned int				aPlayerID,
											unsigned int				aBoardId);

	std::string							PrintScoreboard(
											unsigned int				aBoardId);

	std::string							ToString() const;

protected:
	void								Initialize();

private:
	unsigned int						myId;
	std::vector<ScoreRow>				scoredPoints;
};

#ifdef _DEBUG
#define SCB_DEFAULT_ID					static_cast<unsigned int>(GetHashID("SCBTABLE15102344370000000"))
#else
#define SCB_DEFAULT_ID					static_cast<unsigned int>(GetHashID(GenKey("SCBTABLE")))
#endif

