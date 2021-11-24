#pragma once

// Check VS
#if _WIN32 || _WIN64
#if _WIN64
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

#include <iostream>
#include <mutex>
#include <string>

extern std::mutex logMutex;

struct Point
{
	short x = 0;
	short y = 0;

	Point()
	:Point(0,0)
	{
	}

	Point(
		short x, 
		short y)
	{
		this->x = x;
		this->y = y;
	}

	Point operator&=(
		Point& aPoint)
	{
		if (this == _STD addressof(aPoint)) {
			return *this;
		}

		this->x = aPoint.x;
		this->y = aPoint.y;
		return *this;
	}

	friend bool operator==(
		const Point& lp, 
		const Point& rp)
	{
		if (lp.x == rp.x && lp.y == rp.y)
			return true;
		return false;
	}

	friend std::ostream& operator<<(
		std::ostream& os, 
		const Point& obj)
	{
		return os << "[" << obj.x << "," << obj.y << "]";
	}

	friend std::istream& operator<<(
		std::istream& is, 
		const Point& obj)
	{
		return is;
	}
};

char*
GenKey(
	const char* aKeyword);

unsigned int
GetHashID(
	std::string aKeyword);

int
Random(
	int	aFrom,
	int	aTo);

template <typename T>
void
PrivLog(
	const T&		aT)
{
	std::cout << aT << '\n';
}

template<typename T, typename... Args>
void
PrivLog(
	const T&		aT,
	const Args&...	aArgs)
{
	std::cout << aT << ' ';

	PrivLog(aArgs...);
}

template<typename... Args>
void
Log(
	const Args&... aArgs)
{
	std::lock_guard<std::mutex> lock(logMutex);

	PrivLog(aArgs...);
}