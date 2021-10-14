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
	unsigned short x = 0;
	unsigned short y = 0;
};

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