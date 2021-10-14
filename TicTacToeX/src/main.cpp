// This file contains the 'main' function. Program execution begins and ends there.

#include "../lib/MessageBus.h"
#include "../lib/Threads.h"

#include "WorkerCompanion.h"

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
	//PerformOneThousandHumans();

	Union un2;
	un2.ec = EnumClass::Three;

	std::cout << "un2.i = " << un2.i << std::endl;

	//convert from int to strongly typed enum
	Union un;
	un.i = 0;

	if (un.ec == EnumClass::Zero) std::cout << "True" << std::endl;

	typedef std::vector<int> Row;
	std::unique_ptr<Row[]> theBoard;
	int size = 3;

	Row r(3, 0);
	r[0];

	theBoard = std::unique_ptr<Row[]>(new Row[size]);
	for (size_t i = 0; i < size; ++i)
		theBoard[i] = Row(size, 0);

	std::cout << theBoard[1][1];
	theBoard[1][1] += 3;
	std::cout << theBoard[1][1];

	double timestamp = std::chrono::system_clock::now().time_since_epoch().count();

	std::string text = "aaaaabbbbccc";
	std::vector<int> count(25, 0);
	for (int i = 0; i < text.length(); i++)
	{
		int index = text.at(i) - 97;
		count[index]++;
	}

	std::string res = "";
	for (int i = 0; i < 25; i++)
	{
		if (count[i] % 2 != 0)
		{
			char letter = i + 97;
			res.push_back(letter);
			res.push_back(',');
		}
	}

	if (res.length() > 0)
	{
		res.pop_back();
	}

	else
		res = "EMPTY";

	return 0;
}