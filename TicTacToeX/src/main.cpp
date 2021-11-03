// This file contains the 'main' function. Program execution begins and ends there.

#include "../lib/MessageBus.h"
#include "../lib/Threads.h"

#include "WorkerCompanion.h"

#include <map>

enum class EnumClass : int //set size for enum
{
	Zero, One, Two, Three, Four
};

union Union //This will allow us to convert
{
	EnumClass ec;
	int i;
};

int main()
{
	std::map<short, Point>	boardMap;

	int size = 5;
	int k = 1;
	for (int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
			boardMap.insert(std::pair<int, Point>(k++, Point(i,j)));
	}

	Point pt = boardMap.at(2);

	for (std::pair<int, Point> p : boardMap)
	{
		std::cout << "<" << p.first << ": " << p.second << ">" << std::endl;
	}

	system("CLS");

	std::vector<Point> marked0 =
	{
		Point(0,2), Point(0,0),	Point(0,1),
		Point(2,0), Point(2,2), Point(1,0),
		Point(1,1), Point(2,1), Point(1,2)
	};
	std::vector<Point> marked;
	std::vector<Point> ignore = { Point(), Point(0,2), Point(2,0), Point(2,2) };

	auto pred = [ignore](Point& p)
	{
		return (std::ranges::find(ignore, p) == ignore.end());
	};

	std::ranges::copy_if(
		marked0,
		std::back_inserter(marked),
		pred
	);
	

	Union un2;
	un2.ec = EnumClass::Three;

	std::cout << "un2.i = " << un2.i << std::endl;

	//convert from int to strongly typed enum
	Union un;
	un.i = 0;

	if (un.ec == EnumClass::Zero) std::cout << "True" << std::endl;

	typedef std::vector<int> Row;
	std::unique_ptr<Row[]> theBoard;
	size = 3;

	Row r(3, 0);
	r[0];

	theBoard = std::unique_ptr<Row[]>(new Row[size]);
	for (size_t i = 0; i < size; ++i)
		theBoard[i] = Row(size, 0);

	std::cout << theBoard[1][1];
	theBoard[1][1] += 3;
	std::cout << theBoard[1][1];

	double timestamp = std::chrono::system_clock::now().time_since_epoch().count();

	return 0;
}