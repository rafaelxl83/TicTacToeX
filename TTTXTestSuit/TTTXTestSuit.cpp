#include "pch.h"
#include "CppUnitTest.h"

#include <numeric>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>


#define private public
#define protected public

#pragma region "Project libs"
#include "..\TicTacToeX\lib\Utils.h"
#include "..\TicTacToeX\src\Symbol.h"
#include "..\TicTacToeX\src\EventTable.h"
#include "..\TicTacToeX\src\Board.h"
#include "..\TicTacToeX\src\Player.h"
#include "..\TicTacToeX\src\GamePlay.h"
#pragma endregion

#define TEST_DEFAULT_ID					static_cast<unsigned int>(GetHashID(GenKey("TEST")))

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TTTXTestSuit
{

#pragma region "Test Helper"
	class Helper
	{
	public:

#pragma region "Log Helper"
		static void
			Trace(
				const char* file,
				const int line,
				const WCHAR* pFormat, ...)
		{
			WCHAR buffer[1024] = { 0 };
			int StringLength = wsprintf(buffer, L"%hs(%d): ", file, line);

			va_list AdditionalParameters;
			va_start(AdditionalParameters, pFormat);
			wvsprintf(buffer + StringLength, pFormat, AdditionalParameters);
			va_end(AdditionalParameters);

			OutputDebugString(buffer);
		}

		static void
			TestOutput(
				const WCHAR* pFormat, ...)
		{
			WCHAR   buffer[1024] = { 0 };

			va_list AdditionalParameters;
			va_start(AdditionalParameters, pFormat);
			wvsprintf(buffer, pFormat, AdditionalParameters);
			va_end(AdditionalParameters);

			Logger::WriteMessage(buffer);
		}

#ifdef UNICODE
		static WCHAR*
			mbs2wcs(
				char* c)
		{
			size_t i = 0, size = strlen(c) + 1;
			WCHAR* tmp = new WCHAR[size];

			mbstowcs_s(&i, tmp, size, c, size);
			return tmp;
		}
		static char*
			wcs2mbs(
				LPWSTR lpw)
		{
			char* tmp = (char*)malloc(256);
			size_t i;

			wcstombs_s(&i, tmp, (size_t)256, lpw, (size_t)256);
			return tmp;
		}

		static void
			mbs2wcs_s(
				char* c,
				WCHAR** lpw)
		{
			size_t i = 0, size = strlen(c) + 1;
			*lpw = new WCHAR[size];

			mbstowcs_s(&i, *lpw, size, c, size);
		}
		static void
			wcs2mbs_s(
				LPWSTR lpw,
				char** c)
		{
			*c = (char*)malloc(256);
			size_t i;

			wcstombs_s(&i, *c, (size_t)256, lpw, (size_t)256);
		}
#endif

#pragma endregion

#pragma region "Game basic items for test Helper"
		class GameBase4Test
		{
		public:
			GameBase4Test() 
			{ 
				InitializeGame(); 
			}

			void InitializeGame()
			{
				char player_id[128];
				char tmp_id[128];
				double timestamp;

				timestamp = std::chrono::system_clock::now().time_since_epoch().count();
				sprintf_s(tmp_id, "EVTTABLE%.lf", timestamp);
				evtID = GetHashID(tmp_id);
				evt = EventTable();

				timestamp = std::chrono::system_clock::now().time_since_epoch().count();
				sprintf_s(tmp_id, "BOARD%.lf", timestamp);
				boardID = GetHashID(tmp_id);
				board = new Board(boardID);

				timestamp = std::chrono::system_clock::now().time_since_epoch().count();
				sprintf_s(player_id, "PLAYERID%.lf", timestamp);
				p1ID = GetHashID(player_id);
				s1 = new Symbol(1);

				timestamp = std::chrono::system_clock::now().time_since_epoch().count();
				sprintf_s(player_id, "PLAYERID%.lf", timestamp + 1);
				p2ID = GetHashID(player_id);
				s2 = new Symbol(2);

				timestamp = std::chrono::system_clock::now().time_since_epoch().count();
				sprintf_s(player_id, "PLAYERID%.lf", timestamp + 2);
				p3ID = GetHashID(player_id);
				s3 = new Symbol(3);
			}

			std::string Board3x3ToStr(std::shared_ptr<Row[]> board, char* separator, bool nl)
			{
				std::string line;
				std::stringbuf buffer;
				std::ostream os(&buffer);

				line.insert(0, 15, '-');
				line[4] = '|';
				line[9] = '|';

				for (int i = 0; i < 3; i++)
				{
					std::copy(
						board[i].begin(),
						board[i].end() - 1,
						std::ostream_iterator<Symbol>(os, separator));
					os << (*(board[i].end() - 1));
					
					if(nl && i < 2)
						os << std::endl << line << std::endl;
				}

				return buffer.str();
			}

#pragma region "Emulate Player Events"
			void Emulate1PlayerEvents()
			{
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos1.x = 0; pos1.y = 1;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos1.x = 0; pos1.y = 2;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos1.x = 1; pos1.y = 0;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos1.x = 1; pos1.y = 1;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos1.x = 1; pos1.y = 2;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos1.x = 2; pos1.y = 0;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos1.x = 2; pos1.y = 1;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos1.x = 2; pos1.y = 2;
				evt.RegisterEvent(p1ID, *s1, pos1);
			}

			void Emulate2PlayerEvents()
			{
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 0; pos2.y = 1;
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos1.x = 0; pos1.y = 2;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 1; pos2.y = 0;
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos1.x = 1; pos1.y = 1;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 1; pos2.y = 2;
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos1.x = 2; pos1.y = 0;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 2; pos2.y = 1;
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos1.x = 2; pos1.y = 2;
				evt.RegisterEvent(p1ID, *s1, pos1);
			}

			void Emulate3PlayerEvents()
			{
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 0; pos2.y = 1;
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos3.x = 0; pos3.y = 2;
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 0; pos1.y = 3;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 1; pos2.y = 0;
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos3.x = 1; pos3.y = 1;
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 1; pos1.y = 2;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 1; pos2.y = 3;
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos3.x = 2; pos3.y = 0;
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 2; pos1.y = 1;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 2; pos2.y = 3;
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos3.x = 3; pos3.y = 0;
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 3; pos1.y = 1;
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 3; pos2.y = 3;
				evt.RegisterEvent(p2ID, *s2, pos2);
			}

			unsigned int EmulatePlayerEntry(Player& p, std::iostream& in, int value)
			{
				in << value;
				int move = p.MakeAMove();
				in.get();
				in.clear();
				return move;
			}
#pragma endregion

#pragma region "Emulate Player Board Actions"
			void Emulate1PlayerBoardActions(Board* aBoard)
			{
				if (aBoard == nullptr) return;

				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s", 
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos1.x = 0; pos1.y = 1;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos1.x = 0; pos1.y = 2;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos1.x = 1; pos1.y = 0;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos1.x = 1; pos1.y = 1;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos1.x = 1; pos1.y = 2;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos1.x = 2; pos1.y = 0;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos1.x = 2; pos1.y = 1;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos1.x = 2; pos1.y = 2;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate1PlayerBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));
			}

			void Emulate2PlayersBoardActions(Board* aBoard)
			{
				TEST_OUTPUT(L"Emulate2PlayersBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 0; pos2.y = 1;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);

				pos1.x = 0; pos1.y = 2;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 1; pos2.y = 0;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);

				pos1.x = 1; pos1.y = 1;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 1; pos2.y = 2;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);

				pos1.x = 2; pos1.y = 0;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 2; pos2.y = 1;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);

				pos1.x = 2; pos1.y = 2;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				TEST_OUTPUT(L"Emulate2PlayersBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));
			}

			void Emulate3PlayersBoardActions(Board* aBoard)
			{
				TEST_OUTPUT(L"Emulate3PlayersBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 0; pos2.y = 1;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos3.x = 0; pos3.y = 2;
				aBoard->SetMark(pos3, *s3);
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 0; pos1.y = 3;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 1; pos2.y = 0;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos3.x = 1; pos3.y = 1;
				aBoard->SetMark(pos3, *s3);
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 1; pos1.y = 2;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 1; pos2.y = 3;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);

				TEST_OUTPUT(L"Emulate3PlayersBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));

				pos3.x = 2; pos3.y = 0;
				aBoard->SetMark(pos3, *s3);
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 2; pos1.y = 1;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 2; pos2.y = 2;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos3.x = 2; pos3.y = 3;
				aBoard->SetMark(pos3, *s3);
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 3; pos1.y = 0;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);
				pos2.x = 3; pos2.y = 1;
				aBoard->SetMark(pos2, *s2);
				evt.RegisterEvent(p2ID, *s2, pos2);
				pos3.x = 3; pos3.y = 2;
				aBoard->SetMark(pos3, *s3);
				evt.RegisterEvent(p3ID, *s3, pos3);

				pos1.x = 3; pos1.y = 3;
				aBoard->SetMark(pos1, *s1);
				evt.RegisterEvent(p1ID, *s1, pos1);

				TEST_OUTPUT(L"Emulate3PlayersBoardActions: %s",
					Helper::mbs2wcs((char*)aBoard->ToString().c_str()));
			}
#pragma endregion

#pragma region "Base test variables"
			EventTable evt;
			unsigned int evtID;

			Board* board;
			unsigned int boardID;

			Symbol* s1;
			Point pos1;
			unsigned int p1ID;

			Symbol* s2;
			Point pos2;
			unsigned int p2ID;

			Symbol* s3;
			Point pos3;
			unsigned int p3ID;
#pragma endregion
		};
#pragma endregion
	};
#pragma endregion

#pragma region "Test Symbol Class"
	TEST_CLASS(SymbolUnitTest)
	{
	public:

#pragma region "Initialize and cleanup tests"
		TEST_METHOD_INITIALIZE(SymbolInitialize)
		{
			// method initialization code
		}

		TEST_METHOD_CLEANUP(SymbolCleanup)
		{
			// test method cleanup  code
		}
#pragma endregion

#pragma region "Simbol tests[X,O,M,S,H]"
		TEST_METHOD(SymbolX)
		{
			Symbol* s = nullptr;
			s = new Symbol(1);

			TEST_OUTPUT(L"Symbol X: char X [%c]", s->GetProperty().symbol);
			Assert::AreEqual(s->GetProperty().ico, 'X');
		}

		TEST_METHOD(SymbolO)
		{
			Symbol* s = nullptr;
			s = new Symbol(2);

			TEST_OUTPUT(L"Symbol O: value 3 [%i]", s->GetProperty().value);
			Assert::AreEqual(s->GetProperty().value, 3);
		}

		TEST_METHOD(SymbolM)
		{
			Symbol* s = nullptr;
			s = new Symbol(3);

			Symbol::Union u;
			u.s = Symbol::AvailableSymbols::M;

			TEST_OUTPUT(L"Symbol M: victory points 18 [%i]", s->GetProperty().victory);
			Assert::AreEqual(s->GetProperty().victory, 3 * u.i);
		}

		TEST_METHOD(SymbolS)
		{
			Symbol s(4);
			Symbol::Union u;
			u.s = Symbol::AvailableSymbols::S;

			TEST_OUTPUT(L"Symbol S: value 10 [%i]", s.GetProperty().value);
			Assert::AreEqual(s.GetProperty().value, u.i);
		}

		TEST_METHOD(SymbolH)
		{
			Symbol s(5);
			Symbol::Union u;
			u.s = Symbol::AvailableSymbols::H;

			TEST_OUTPUT(L"Symbol H: symbol H [%c]", s.GetProperty().symbol);
			Assert::AreEqual(s.GetProperty().ico, 'H');
		}

		TEST_METHOD(SymbolCasting)
		{
			Symbol s(Symbol::AvailableSymbols::O);
			Symbol symbol = (Symbol::AvailableSymbols)3;

			TEST_OUTPUT(L"Symbol Casting: symbol O [%c]", s.GetProperty().symbol);
			Assert::AreEqual(s.GetProperty().value, symbol.GetProperty().value);
		}
		
#pragma endregion
	
	};
#pragma endregion

#pragma region "Event Table Unit Test Class"
	TEST_CLASS(EventTableUnitTest)
	{
	public:
		EventTable evt;
		unsigned int p1ID;
		unsigned int p2ID;
		unsigned int p3ID;

#pragma region "Initialize and cleanup tests"
		TEST_METHOD_INITIALIZE(Startup)
		{
			//time_t rawtime = time(0), datetime;
			//struct tm timeinfo;
			//int year = 2017, month = 11, day = 9;

			///* get current timeinfo and modify it to the user's choice */
			////time(&rawtime);
			//localtime_s(&timeinfo, &rawtime);
			//timeinfo.tm_year = year - 1900;
			//timeinfo.tm_mon = month - 1;
			//timeinfo.tm_mday = day;
			//timeinfo.tm_hour = 10;
			//timeinfo.tm_min = 33;

			///* call mktime: timeinfo->tm_wday will be set */
			//datetime = mktime(&timeinfo);
			//double timestamp = std::chrono::system_clock::from_time_t(datetime).time_since_epoch().count();

			// method initialization code
			char player_id[128];
			char evt_id[128];
			double timestamp;

			timestamp = std::chrono::system_clock::now().time_since_epoch().count();
			sprintf_s(evt_id, "EVTTABLE%.lf", timestamp);
			evt = EventTable(GetHashID(evt_id));

			timestamp = std::chrono::system_clock::now().time_since_epoch().count();
			sprintf_s(player_id, "PLAYERID%.lf", timestamp);
			p1ID = GetHashID(player_id);

			timestamp = std::chrono::system_clock::now().time_since_epoch().count();
			sprintf_s(player_id, "PLAYERID%.lf", timestamp + 1);
			p2ID = GetHashID(player_id);

			timestamp = std::chrono::system_clock::now().time_since_epoch().count();
			sprintf_s(player_id, "PLAYERID%.lf", timestamp + 2);
			p3ID = GetHashID(player_id);
		}

		TEST_METHOD_CLEANUP(End)
		{
			// test method cleanup  code
			evt.~EventTable();
		}
#pragma endregion

#pragma region "Unit Tests"
		TEST_METHOD(RegisterEventTest)
		{
			Symbol s1(1);
			Point p1;

			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 0; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 0; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 1; p1.y = 0;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 1; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 1; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 2; p1.y = 0;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 2; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 2; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);

			std::string str = evt.ToString();
			TEST_OUTPUT(L"RegisterEvent: %s", Helper::mbs2wcs((char*)str.c_str()));

			Assert::AreEqual((int)evt.allPlayedEvents[8].position.y, (int)p1.y);
		}

		TEST_METHOD(GetAllEventsTest)
		{
			Symbol s1(1);
			Point p1;

			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 0; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 0; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 1; p1.y = 0;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 1; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 1; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 2; p1.y = 0;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 2; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p1.x = 2; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);

			std::weak_ptr<std::vector<EventTable::EventRow>> all = 
				std::make_shared<std::vector<EventTable::EventRow>>(evt.GetAllEvents());
			std::vector<EventTable::EventRow> r = !all.expired() ? *all.lock() : evt.GetAllEvents();

			EventTable::EventRow last = *--r.end();
			Assert::AreEqual((int)last.position.y, (int)p1.y);
		}

		TEST_METHOD(Get1PlayerEventsTest)
		{
			Symbol s1(1);
			Symbol s2(2);

			Point p1;
			Point p2;

			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 0; p2.y = 1;
			evt.RegisterEvent(p2ID, s2, p2);
			p1.x = 0; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 1; p2.y = 0;
			evt.RegisterEvent(p2ID, s2, p2);
			p1.x = 1; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 1; p2.y = 2;
			evt.RegisterEvent(p2ID, s2, p2);
			p1.x = 2; p1.y = 0;
			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 2; p2.y = 1;
			evt.RegisterEvent(p2ID, s2, p2);
			p1.x = 2; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);

			std::vector<EventTable::EventRow> r = evt.GetPlayersEvents(p2ID);

			char buffer[128];
			for (EventTable::EventRow &e : r)
			{
				sprintf_s(buffer, "timestamp [%.lf], player [%u]", e.timestamp, e.playerID);
				TEST_OUTPUT(L"Get1PlayerEventsTest: %s", Helper::mbs2wcs(buffer));
			}

			EventTable::EventRow last = *--r.end();
			Assert::AreEqual(last.playerID, p2ID);
		}

		TEST_METHOD(Get2PlayersEventsTest)
		{
			Symbol s1(1);
			Symbol s2(2);
			Symbol s3(3);

			Point p1;
			Point p2;
			Point p3;

			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 0; p2.y = 1;
			evt.RegisterEvent(p2ID, s2, p2);
			p3.x = 0; p3.y = 2;
			evt.RegisterEvent(p3ID, s3, p3);

			p1.x = 0; p1.y = 3;
			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 1; p2.y = 0;
			evt.RegisterEvent(p2ID, s2, p2);
			p3.x = 1; p3.y = 1;
			evt.RegisterEvent(p3ID, s3, p3);

			p1.x = 1; p1.y = 2;
			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 1; p2.y = 3;
			evt.RegisterEvent(p2ID, s2, p2);
			p3.x = 2; p3.y = 0;
			evt.RegisterEvent(p3ID, s3, p3);

			p1.x = 2; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 2; p2.y = 3;
			evt.RegisterEvent(p2ID, s2, p2);
			p3.x = 3; p3.y = 0;
			evt.RegisterEvent(p3ID, s3, p3);

			p1.x = 3; p1.y = 1;
			evt.RegisterEvent(p1ID, s1, p1);
			p2.x = 3; p2.y = 3;
			evt.RegisterEvent(p2ID, s2, p2);

			// 5p2ID 4p3ID
			std::vector<EventTable::EventRow> r = evt.GetPlayersEvents({ p2ID, p3ID });

			char buffer[128];
			for (EventTable::EventRow& e : r)
			{
				sprintf_s(buffer, "timestamp [%.lf], player [%u]", e.timestamp, e.playerID);
				TEST_OUTPUT(L"Get2PlayerEventsTest: %s", Helper::mbs2wcs(buffer));
			}

			EventTable::EventRow last = *--r.end();
			Assert::AreEqual(last.playerID, p2ID);
		}
#pragma endregion
	};
#pragma endregion

#pragma region "Board Unit Test"
	TEST_CLASS(BoardUnitTest)
	{
	public:
		std::shared_ptr<Helper::GameBase4Test> emu;
		Board* board;
		char board_id[128];

#pragma region "Initialize and cleanup tests"
		TEST_METHOD_INITIALIZE(Startup)
		{
			// method initialization code
			emu = std::make_shared<Helper::GameBase4Test>(Helper::GameBase4Test());
			sprintf_s(board_id, "%s", GenKey("BOARD"));
		}

		TEST_METHOD_CLEANUP(End)
		{
			// test method cleanup  code
		}
#pragma endregion

#pragma region "Unit Tests"
		TEST_METHOD(DefaultBoardTest)
		{
			board = new Board(GetHashID(board_id));
			Assert::AreEqual((int)board->theBoard.get()->size(), 7);
		}

		TEST_METHOD(BoardPointTest)
		{
			board = new Board(GetHashID(board_id));
			bool isNull = false;

			std::optional<Point> p = board->GetPoint(2);
			p = board->GetPoint(30);

			if (!p.has_value())
				isNull = true;

			Assert::AreEqual(isNull, true);
		}

		TEST_METHOD(BoardTestMark)
		{
			board = new Board(GetHashID(board_id));
			emu->Emulate1PlayerBoardActions(board);

			bool markSucceeded = board->SetMark(
				emu->evt.allPlayedEvents[0].position, 
				Symbol::AvailableSymbols::X);

			Assert::AreEqual(markSucceeded, false);
		}

		TEST_METHOD(BoardTestTwoPlayers)
		{
			board = new Board(GetHashID(board_id));

			bool completed = board->IsFull();

			emu->Emulate2PlayersBoardActions(board);

			completed = board->IsFull();

			Assert::AreEqual(completed, true);
		}

		TEST_METHOD(BoardTestThreePlayers)
		{
			board = new Board(
				BoardSizes::ThreePlayers,
				GetHashID(board_id));

			emu->Emulate3PlayersBoardActions(board);

			std::weak_ptr<Row[]> aSection = board->GetSector(Point());

			std::string str = emu->Board3x3ToStr(aSection.lock(), (char*)"", false);

			Assert::AreEqual(str.c_str(), " [255] [255] [255] [255]X[1]O[3] [255]O[3]M[6]");
		}

		TEST_METHOD(BoardTestSectionDiagonal)
		{
			board = new Board(
				BoardSizes::ThreePlayers,
				GetHashID(board_id));

			std::stringbuf buffer;
			std::ostream os(&buffer);
			board->PrintBoard(os);

			std::string s = buffer.str();

			emu->Emulate3PlayersBoardActions(board);

			buffer.str("");
			board->PrintBoard(os);
			s = buffer.str();

			std::weak_ptr<Row[]> aSection = board->GetSector(Point(1,1));
			std::string str = emu->Board3x3ToStr(aSection.lock(), (char*)"|", true);
			TEST_OUTPUT(L"BoardTestSectionDiagonal: \n%s",
				Helper::mbs2wcs((char*)str.c_str()));

			aSection = board->GetSector(Point(2, 2));
			str = emu->Board3x3ToStr(aSection.lock(), (char*)"|", true);
			TEST_OUTPUT(L"BoardTestSectionDiagonal: \n%s",
				Helper::mbs2wcs((char*)str.c_str()));

			aSection = board->GetSector(Point(3, 3));
			str = emu->Board3x3ToStr(aSection.lock(), (char*)"", false);

			Assert::AreEqual(str.c_str(), "O[3]M[6] [255]M[6]X[1] [255] [255] [255] [255]");
		}
#pragma endregion
	};
#pragma endregion

#pragma region "Player Unit Test"
	TEST_CLASS(PlayerUnitTests)
	{
	public:
		std::shared_ptr<Helper::GameBase4Test> emu;
		Player* player;
		char player_id[128];

#pragma region "Initialize and cleanup tests"
		TEST_METHOD_INITIALIZE(Startup)
		{
			// method initialization code
			emu = std::make_shared<Helper::GameBase4Test>(Helper::GameBase4Test());
			sprintf_s(player_id, "%s", GenKey("Player"));
		}

		TEST_METHOD_CLEANUP(End)
		{
			// test method cleanup  code
		}
#pragma endregion

#pragma region "Unit Tests"
		TEST_METHOD(PlayerCreationTest)
		{
			player = new Player(
				"Momo2017", 
				Symbol(
					Symbol::AvailableSymbols::X));

			Assert::AreEqual(
				(int)player->GetPlayerSymbol().GetProperty().symbol, 
				(int)Symbol::AvailableSymbols::X);
		}

		TEST_METHOD(PlayerMarkTest)
		{
			std::stringbuf buffer;
			std::iostream input(&buffer);

			player = new Player(
				emu->p1ID,
				"Momo20171109",
				Symbol(Symbol::AvailableSymbols::X),
				input);

			player->AddBoardId(emu->boardID);

			unsigned int move = 0, t = 6;
			do
			{
				move = emu->EmulatePlayerEntry(*player, input, t--);
				player->SetState(Player::PlayerState::Turn);
			} while (move != 1);

			player->SetState(Player::PlayerState::Idle);

			Assert::AreEqual((int)move, 1);
		}

		TEST_METHOD(PlayerBoardListTest)
		{
			player = new Player(
				"Momo2017",
				Symbol(Symbol::AvailableSymbols::X));

			unsigned int bIDs[5](0);

			for (int i = 0; i < 5; i++)
				bIDs[i] = GetHashID(GenKey("TEST" + i));

			for (auto b : bIDs)
				player->AddBoardId(b);

			player->RemBoard(2);
			player->RemBoard(bIDs[3]);

			Assert::AreEqual((int)player->GetAllBoards().size(), 3);
		}
#pragma endregion
	};
#pragma endregion

#pragma region "GamePlay Unit Test"
	TEST_CLASS(GamePlayUnitTest)
	{
	public:
		std::shared_ptr<Helper::GameBase4Test> emu;
		GamePlay* gplay;
		Player* player1;
		Player* player2;
		Player* player3;
		Board* board;
		EventTable evt;

		unsigned int player1ID;
		unsigned int player2ID;
		unsigned int player3ID;
		unsigned int boardID;
		unsigned int evtID;

#pragma region "Initialize and cleanup tests"
		TEST_METHOD_INITIALIZE(Startup)
		{
			// method initialization code
			emu = std::make_shared<Helper::GameBase4Test>(Helper::GameBase4Test());
			player1ID = GetHashID(GenKey("PLAYER1"));
			player2ID = GetHashID(GenKey("PLAYER2"));
			player3ID = GetHashID(GenKey("PLAYER3"));
			boardID = GetHashID(GenKey("BOARD"));
			evtID = GetHashID(GenKey("EVT"));
		}

		TEST_METHOD_CLEANUP(End)
		{
			// test method cleanup  code
		}
#pragma endregion

#pragma region "Unit Tests"
		TEST_METHOD(EvaluateTest)
		{
			gplay = new GamePlay();

			int victory = 0;
			Assert::AreEqual(gplay->Evaluate(victory), 0);

			victory = (int)Symbol::AvailableSymbols::X * 3;
			Assert::AreEqual(gplay->Evaluate(victory), 1);

			victory = (int)Symbol::AvailableSymbols::O * 3;
			Assert::AreEqual(gplay->Evaluate(victory), 2);

			victory = (int)Symbol::AvailableSymbols::M * 3;
			Assert::AreEqual(gplay->Evaluate(victory), 3);

			victory = (int)Symbol::AvailableSymbols::S * 3;
			Assert::AreEqual(gplay->Evaluate(victory), 4);

			victory = (int)Symbol::AvailableSymbols::H * 3;
			Assert::AreEqual(gplay->Evaluate(victory), 5);

			victory = 200;
			Assert::AreEqual(gplay->Evaluate(victory), 0);
		}

		TEST_METHOD(CheckSection2PD1WinnerTest)
		{
			int winner = 0;
			gplay = new GamePlay();
			board = new Board(boardID);
			
			emu->Emulate2PlayersBoardActions(board);

			for (Point p : board->GetMarkedPositions())
			{
				winner = gplay->CheckSection(*board, p);
				if (winner != 0)
					break;
			}
			Assert::AreEqual(winner, 1);
		}

		TEST_METHOD(CheckSection3PD2WinnerTest)
		{
			int winner = 0;
			Point winnerPoint;
			gplay = new GamePlay();
			board = new Board(
				BoardSizes::ThreePlayers,
				boardID);

			emu->Emulate3PlayersBoardActions(board);
			for (Point p : board->GetMarkedPositions())
			{
				winner = gplay->CheckSection(*board, p);
				if (winner != 0)
				{
					winnerPoint = p;
					break;
				}
			}

			std::weak_ptr<Row[]> aSection = board->GetSector(winnerPoint);
			std::string str = emu->Board3x3ToStr(aSection.lock(), (char*)"|", true);
			TEST_OUTPUT(L"CheckSection3PD2WinnerTest: \n%s",
				Helper::mbs2wcs((char*)str.c_str()));

			Assert::AreEqual(winner, 3);
		}

		TEST_METHOD(PlaythroughTest)
		{
			Symbol winner = 0;
			int index = 0;

			std::stringbuf buffer;
			std::iostream input(&buffer);

			gplay = new GamePlay();
			gplay->AddBoard(
				new Board(boardID));

			player1 = new Player(
				player1ID,
				"PLAYER1",
				Symbol(Symbol::AvailableSymbols::empty),
				input);

			player2 = new Player(
				player2ID,
				"PLAYER2",
				Symbol(Symbol::AvailableSymbols::empty),
				input);
			
			player1->SetSymbol(Symbol(1));
			player2->SetSymbol(Symbol(2));

			player1->SetState(Player::PlayerState::Turn);

			//std::optional<Board*> b = gplay->GetBoard(0);
			std::optional<Board*> b = gplay->GetBoard(boardID);

			if (b.has_value())
			{
				board = b.value();
				do
				{
					index = Random(1, 9);

					if (player1->GetState() == Player::PlayerState::Turn)
					{
						if (board->SetMark((short)index, player1->GetPlayerSymbol()))
						{
							player1->SetState(Player::PlayerState::Idle);
							player2->SetState(Player::PlayerState::Turn);
						}
					}
					else
					{
						if (board->SetMark((short)index, player2->GetPlayerSymbol()))
						{
							player2->SetState(Player::PlayerState::Idle);
							player1->SetState(Player::PlayerState::Turn);
						}
					}

					winner = gplay->HasWinner(*board);
				} while (winner == Symbol::AvailableSymbols::empty && !board->IsFull());

				std::string str = emu->Board3x3ToStr(
					board->GetSector(Point(1, 1)).lock(), (char*)"|", true);
				TEST_OUTPUT(L"PlaythroughTest: %c - \n%s", winner.GetProperty().ico,
					Helper::mbs2wcs((char*)str.c_str()));
			}

			Assert::AreEqual(board->GetID(), boardID);
		}
#pragma endregion
	};
#pragma endregion

#pragma region "Unit Test Class Template"
	TEST_CLASS(TTTXTestSuit)
	{
	public:

#pragma region "Initialize and cleanup tests"
		TEST_METHOD_INITIALIZE(Startup)
		{
			// method initialization code
		}

		TEST_METHOD_CLEANUP(End)
		{
			// test method cleanup  code
		}
#pragma endregion

#pragma region "Unit Tests"
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual("", "");
		}
#pragma endregion
	};
#pragma endregion

}
