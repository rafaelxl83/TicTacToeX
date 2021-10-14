#include "pch.h"
#include "CppUnitTest.h"

#include <numeric>
#include <chrono>


#define private public
#define protected public

#pragma region "Project libs"
#include "..\TicTacToeX\lib\Utils.h"
#include "..\TicTacToeX\src\Symbol.h"
#include "..\TicTacToeX\src\EventTable.h"
#pragma endregion

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
			Assert::AreEqual(s->GetProperty().symbol, 'X');
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
			Assert::AreEqual(s.GetProperty().symbol, 'H');
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

			std::weak_ptr<std::vector<EventRow>> all = std::make_shared<std::vector<EventRow>>(evt.GetAllEvents());
			std::vector<EventRow> r = !all.expired() ? *all.lock() : evt.GetAllEvents();

			EventRow last = *--r.end();
			Assert::AreEqual((int)last.position.y, (int)p1.y);
		}

		TEST_METHOD(GetPlayerEventsTest)
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

			std::vector<EventRow> r = evt.GetPlayerEvents(p2ID);

			char buffer[128];
			for (EventRow &e : r)
			{
				sprintf_s(buffer, "timestamp [%.lf], player [%u]", e.timestamp, e.playerID);
				TEST_OUTPUT(L"GetPlayerEventsTest: %s", Helper::mbs2wcs(buffer));
			}

			EventRow last = *--r.end();
			Assert::AreEqual(last.playerID, p2ID);
		}

		TEST_METHOD(GetPlayersEventsTest)
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
			std::vector<EventRow> r = evt.GetPlayersEvents({ p2ID, p3ID });

			char buffer[128];
			for (EventRow& e : r)
			{
				sprintf_s(buffer, "timestamp [%.lf], player [%u]", e.timestamp, e.playerID);
				TEST_OUTPUT(L"GetPlayersEventsTest: %s", Helper::mbs2wcs(buffer));
			}

			EventRow last = *--r.end();
			Assert::AreEqual(last.playerID, p2ID);
		}
#pragma endregion
	};
#pragma endregion

#pragma region "Board Unit Test"
	TEST_CLASS(BoardUnitTest)
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
		}
#pragma endregion
	};
#pragma endregion

}
