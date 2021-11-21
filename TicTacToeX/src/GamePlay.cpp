#include "GamePlay.h"

#include "../lib/MessageBus.h"

#include "ExceptionHelper.h"

#pragma region "Message Events"
void
GamePlay::OnMessageEndOfGame(
	const MessageEndOfGame&			aMessage) noexcept
{
	try
	{
		if (aMessage.callerId != myId)
			return;

		if (!shutDown)
		{
			// notify the players about the end
			// and send the winner if there are
			// one
			SEND_TO_PLAYERS(MessageEndOfGame(
				aMessage.callerId,
				aMessage.theWinner));

			SEND_TO_GAMEPLAY(MessageShutdown(
				aMessage.callerId));

			Log("[GamePlay]", "OnMessageEndOfGame");
		}
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageEndOfGame", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageStartOfGame(
	const MessageStartOfGame&		aMessage) noexcept
{
	try
	{
		if (aMessage.callerId != myId)
			return;

		SEND_TO_GAMEPLAY(MessageTurnChanged(
			aMessage.callerId,
			aMessage.myPlayerId,
			aMessage.myBoardId,
			false));
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageStartOfGame", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageTurnChanged(
	const MessageTurnChanged&		aMessage) noexcept
{
	try
	{
		if (aMessage.callerId != myId)
			return;

		// cyclic event to manage the playthrough
		if (!shutDown)
		{
			std::optional<Board*> b = GetBoard(aMessage.myBoardId);
			if (!b.has_value())
			{
				Log("[GamePlay]", "OnMessageTurnChanged",
					"ERROR", "Board not found!");
				return;
			}

			if (b.value()->HasUpdate())
			{
				system("CLS");
				std::cout << PrintBoard(aMessage.myBoardId) << std::endl;

				// check if there are a winner
				theWinner = HasWinner(*b.value());
			}

			// If exist a winner, compute the victory ponts
			// and call the end of the game for this board
			if (theWinner != Symbol::AvailableSymbols::empty)
			{
				SEND_TO_GAMEPLAY(MessageScorePoints(
					aMessage.callerId,
					aMessage.myPlayerId,
					aMessage.myBoardId,
					(int)ScorePoints::WINNER,
					(int)theWinner.GetProperty().symbol));

				SEND_TO_GAMEPLAY(MessageEndOfGame(
					aMessage.callerId,
					aMessage.myBoardId,
					(int)theWinner.GetProperty().symbol));
			}
			// No winner, check if this board is full and
			// keep running the playthrough
			else if(!b.value()->IsFull())
				SEND_TO_GAMEPLAY(MessageTurnChanged(
					aMessage.callerId,
					aMessage.myPlayerId,
					aMessage.myBoardId,
					myTurnCounter));
			// the board is full, no winners, so
			// it is the end of the playthrough
			// for this board
			else
				SEND_TO_GAMEPLAY(MessageEndOfGame(
					aMessage.callerId,
					aMessage.myBoardId,
					(int)Symbol::AvailableSymbols::empty));
		}
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageTurnChanged", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageSingleMove(
	const MessageSingleMove&		aMessage) noexcept
{
	try
	{
		if (aMessage.callerId != myId)
			return;

		// must check if the movment is valid
		// and mark it into the correct board
		// if it's not a valid marked position
		// should notify the players about
		// this invalid movment and waint to
		// a new entry from the player
		if (!shutDown)
		{
			std::optional<Board*> b = GetBoard(aMessage.myBoardId);
			if (!b.has_value())
			{
				Log("[GamePlay]", "OnMessageSingleMove", 
					"ERROR", "Board not found!");
				return;
			}

			bool marked = false;
			std::optional<Point> p = b.value()->GetPoint(aMessage.mark);
			if (p.has_value())
			{
				Symbol s = (Symbol::AvailableSymbols)aMessage.symbol;

				std::optional<EventTable*> e = GetEvtTable(aMessage.myBoardId);
				if (e.has_value())
					e.value()->RegisterEvent(aMessage.myPlayerId, s, p.value());
				else
					Log("[GamePlay]", "OnMessageSingleMove",
						"WARNNING", "No event table found for this board");

				marked = b.value()->SetMark(p.value(), s);
			}
			else
				Log("[GamePlay]", "OnMessageSingleMove",
					"ERROR", "Invalid position for this board");

			if(marked)
			{
				Turn();
				SEND_TO_GAMEPLAY(MessageScorePoints(
					aMessage.callerId,
					aMessage.myPlayerId,
					aMessage.myBoardId,
					(int)ScorePoints::SMOVE,
					aMessage.symbol));

				// get the next player input
				SEND_TO_PLAYERS(MessageTurnChanged(
					aMessage.callerId, 
					(int)TurnActions::Next));
			}
			else
			{
				// get the same player input again
				SEND_TO_PLAYERS(MessageTurnChanged(
					aMessage.callerId,
					(int)TurnActions::Retake));
				Log("[GamePlay]", "OnMessageSingleMove",
					"ERROR", "Invalid position for this board");
			}
		}
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageSingleMove", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageBlockMove(
	const MessageBlockMove&			aMessage) noexcept
{
	try
	{
		if (aMessage.callerId != myId)
			return;
		if (!shutDown)
		{
			SEND_TO_GAMEPLAY(MessageScorePoints(
				aMessage.callerId,
				aMessage.myPlayerId,
				aMessage.myBoardId,
				(int)ScorePoints::BMOVE,
				aMessage.symbol));
		}

	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageBlockMove", "Exception", ex.what());
	}
}
void
GamePlay::OnMessageScorePoints(
	const MessageScorePoints&		aMessage) noexcept
{
	try
	{
		if (aMessage.callerId != myId)
			return;

		if (!shutDown)
		{
			// Register the points 
			std::cout << "Player: [" << Symbol((Symbol::AvailableSymbols)aMessage.symbol);
			std::cout << "] Scored points: [" << aMessage.points << "]" << std::endl;
		}

	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageScorePoints", "Exception", ex.what());
	}
}

void
GamePlay::OnMessageShutdown(
	const MessageShutdown&			aMessage) noexcept
{
	try
	{
		if (aMessage.callerId != myId)
			return;

		ShutDown();
	}
	catch (std::system_error& ex)
	{
		Log("[GamePlay]", "OnMessageShutdown", "Exception", ex.what());
	}
}
#pragma endregion

#pragma region "Commom methods"
GamePlay::GamePlay()
	: GamePlay(
		GPLAY_DEFAULT_ID,
		std::vector<Board*>())
{

}
GamePlay::GamePlay(
	const unsigned int			anId)
	: GamePlay(
		anId,
		std::vector<Board*>())
{

}
GamePlay::GamePlay(
	std::vector<Board*>				allBoards)
	: GamePlay(
		GPLAY_DEFAULT_ID,
		allBoards)
{

}
GamePlay::GamePlay(
	const unsigned int			anId,
	std::vector<Board*>				allBoards)
	: myId(anId)
	, myPlayersCount(0)
	, myTurnCounter(0)
	, theWinner(0)
{
	this->AddBoards(allBoards);
}
GamePlay::~GamePlay()
{

}

void
GamePlay::Start()
{
	if (myBoards.empty())
	{
		Log("[GamePlay]", "Start",
			"FATAL", "There are no boards to play!");
		return;
	}

	REGISTER_BOARD(MessageShutdown,			GamePlay::OnMessageShutdown		);
	REGISTER_BOARD(MessageEndOfGame,		GamePlay::OnMessageEndOfGame	);
	REGISTER_BOARD(MessageStartOfGame,		GamePlay::OnMessageStartOfGame	);
	REGISTER_BOARD(MessageTurnChanged,		GamePlay::OnMessageTurnChanged	);
	REGISTER_BOARD(MessageSingleMove,		GamePlay::OnMessageSingleMove	);
	REGISTER_BOARD(MessageBlockMove,		GamePlay::OnMessageBlockMove	);
	REGISTER_BOARD(MessageScorePoints,		GamePlay::OnMessageScorePoints	);

	Initialize();

	SEND_TO_PLAYERS(MessageStartOfGame(myId));
}

void
GamePlay::ShutDown()
{
	shutDown = true;
}

void
GamePlay::Initialize()
{

}

void
GamePlay::Turn()
{

}
#pragma endregion

#pragma region "Board methods"
void
GamePlay::AddBoard(
	Board*						aBoard)
{
	myBoards.push_back(aBoard);
	myEvtTables.push_back(new EventTable(aBoard->GetID()));
}
void
GamePlay::AddBoards(
	std::vector<Board*>			allBoards)
{
	if (allBoards.size() <= 0) return;

	//myBoards = std::move(allBoards); //old method
	std::ranges::move(allBoards, std::back_inserter(myBoards));
	for (auto b : myBoards)
		myEvtTables.push_back(new EventTable(b->GetID()));
}

std::optional<Board*>
GamePlay::GetBoard(
	int							anIndex)
{
	return std::make_optional(myBoards[anIndex]);
}
std::optional<Board*>
GamePlay::GetBoard(
	unsigned int				aBoardId)
{
	auto board = std::ranges::find_if(
		myBoards,
		[aBoardId](Board* b) { return (b->GetID() == aBoardId); });

	if (board != myBoards.end())
		return std::make_optional(*board);

	return std::nullopt;
}
std::vector<Board*>
GamePlay::GetAllBoards()
{
	return myBoards;
}

std::string
GamePlay::PrintBoard(
	int							anIndex)
{
	std::stringbuf buffer;
	std::ostream os(&buffer);

	std::optional<Board*> board = GetBoard(anIndex);
	if (board.has_value())
		board.value()->PrintBoard(os);
	else
		os << "NO BOARD FOUND!";

	return buffer.str();
}
std::string
GamePlay::PrintBoard(
	unsigned int				aBoardId)
{
	std::stringbuf buffer;
	std::ostream os(&buffer);

	std::optional<Board*> board = GetBoard(aBoardId);
	if (board.has_value())
		board.value()->PrintBoard(os);
	else
		os << "NO BOARD FOUND!";

	return buffer.str();
}

bool
GamePlay::SetMark(
	int							anIndex,
	short						aPos,
	Symbol						aSymbol)
{
	bool success = false;
	std::optional<Board*> board = GetBoard(anIndex);

	if (board.has_value())
		success = board.value()->SetMark(aPos, aSymbol);

	return success;
}
bool
GamePlay::SetMark(
	unsigned int				aBoardId,
	short						aPos,
	Symbol						aSymbol)
{
	bool success = false;
	std::optional<Board*> board = GetBoard(aBoardId);

	if (board.has_value())
		success = board.value()->SetMark(aPos, aSymbol);

	return false;
}
#pragma endregion

#pragma region "Event Table methods"
std::optional<EventTable*>
GamePlay::GetEvtTable(
	int							anIndex)
{
	return std::make_optional(myEvtTables[anIndex]);
}
std::optional<EventTable*>
GamePlay::GetEvtTable(
	unsigned int				aTableId)
{
	auto table = std::ranges::find_if(
		myEvtTables,
		[aTableId](EventTable* t) { return (t->GetID() == aTableId); });

	if (table != myEvtTables.end())
		return std::make_optional(*table);

	return std::nullopt;
}
std::vector<EventTable*>
GamePlay::GetAllEvtTables()
{
	return myEvtTables;
}
#pragma endregion

#pragma region "GamePlay Validation"
Symbol
GamePlay::HasWinner(
	Board&						aBoard)
{
	// There are no winners before the 5th mark
	if (aBoard.GetMarkedPositions().size() < 5)
		return Symbol(0);

	int s = 0;
	std::vector<Point> marked = aBoard.GetMarkedPositions();
	//s = aBoard.GetSize() - 1;
	//std::vector<Point> ignore = {Point(), Point(0,s), Point(s,0), Point(s,s)};

	//// No need to check corners
	//// avoid to perform more 32
	//// operations
	//std::ranges::copy_if(
	//	aBoard.GetMarkedPositions(),
	//	std::back_inserter(marked),
	//	[ignore](Point& p)
	//	{
	//		return (std::ranges::find(ignore, p) == ignore.end());
	//	}
	//);

	for (Point p : marked)
	{
		s = CheckSection(aBoard, p);
		if (s != 0)
			break;
	}

	return Symbol(s);
}

int
GamePlay::CheckSection(
	Board& aBoard,
	Point						aPoint)
{
	int sum = 0;
	std::shared_ptr<Row[]> aSection = aBoard.GetSector(aPoint).lock();

#pragma region "Horizontal"
	sum = aSection[0][0].GetProperty().value + 
		  aSection[0][1].GetProperty().value + 
		  aSection[0][2].GetProperty().value;
	sum = Evaluate(sum);
	if (sum > 0)
	{
		Log("[GamePlay]", "CheckSection H1", sum);
		return sum;
	}

	sum = aSection[1][0].GetProperty().value + 
		  aSection[1][1].GetProperty().value + 
		  aSection[1][2].GetProperty().value;
	sum = Evaluate(sum);
	if (sum > 0)
	{
		Log("[GamePlay]", "CheckSection H2", sum);
		return sum;
	}

	sum = aSection[2][0].GetProperty().value + 
		  aSection[2][1].GetProperty().value + 
		  aSection[2][2].GetProperty().value;
	sum = Evaluate(sum);
	if (sum > 0)
	{
		Log("[GamePlay]", "CheckSection H3", sum);
		return sum;
	}
#pragma endregion

#pragma region "Vertical"
	sum = aSection[0][0].GetProperty().value + 
		  aSection[1][0].GetProperty().value + 
		  aSection[2][0].GetProperty().value;
	sum = Evaluate(sum);
	if (sum > 0)
	{
		Log("[GamePlay]", "CheckSection V1", sum);
		return sum;
	}

	sum = aSection[0][1].GetProperty().value + 
		  aSection[1][1].GetProperty().value + 
		  aSection[2][1].GetProperty().value;
	sum = Evaluate(sum);
	if (sum > 0)
	{
		Log("[GamePlay]", "CheckSection V2", sum);
		return sum;
	}

	sum = aSection[0][2].GetProperty().value + 
		  aSection[1][2].GetProperty().value + 
		  aSection[2][2].GetProperty().value;
	sum = Evaluate(sum);
	if (sum > 0)
	{
		Log("[GamePlay]", "CheckSection V3", sum);
		return sum;
	}
#pragma endregion

#pragma region "Diagonal"
	sum = aSection[0][0].GetProperty().value + 
		  aSection[1][1].GetProperty().value + 
		  aSection[2][2].GetProperty().value;
	sum = Evaluate(sum);
	if (sum > 0)
	{
		Log("[GamePlay]", "CheckSection D1", sum);
		return sum;
	}

	sum = aSection[0][2].GetProperty().value + 
		  aSection[1][1].GetProperty().value + 
		  aSection[2][0].GetProperty().value;
	sum = Evaluate(sum);
	if (sum > 0)
	{
		Log("[GamePlay]", "CheckSection D2", sum);
		return sum;
	}
#pragma endregion

	return 0;
}

int
GamePlay::Evaluate(
	int							aValue)
{
	switch (aValue)
	{
		case (int)Symbol::AvailableSymbols::X * 3:
			return 1;
		case (int)Symbol::AvailableSymbols::O * 3:
			return 2;
		case (int)Symbol::AvailableSymbols::M * 3:
			return 3;
		case (int)Symbol::AvailableSymbols::S * 3:
			return 4;
		case (int)Symbol::AvailableSymbols::H * 3:
			return 5;
		default:
			return 0;
	}
}
#pragma endregion