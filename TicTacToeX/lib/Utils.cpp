#include "Utils.h"

#include <random>

std::mutex logMutex;

unsigned int
GetHashID(
	std::string aKeyword)
{
	const char* str = aKeyword.c_str();
#if defined(ENV64BIT)
	if (sizeof(void*) != 8)
	{
		Log("[Utils::GetHashID] ENV64BIT: Error: pointer should be 8 bytes. Exiting.");
		exit(0);
	}

	uint64_t len = aKeyword.length();
	uint64_t result = 14695981039346656037ul;
	for (uint64_t index = 0; index < len; ++index)
	{
		result ^= (uint64_t)str[index];
		result *= 1099511628211ul;
	}
#elif defined (ENV32BIT)
	if (sizeof(void*) != 4)
	{
		Log("[Utils::GetHashID] ENV32BIT: Error: pointer should be 4 bytes. Exiting.");
		exit(0);
	}
	
	uint32_t len = aKeyword.length();
	uint32_t result = 2166136261u;
	for (uint32_t index = 0; index < len; ++index)
	{
		result ^= (uint32_t)str[index];
		result *= 16777619u;
	}
#else
	// I'm only human, and the mistakes above would break the *entire* codebase.
	#error "Must define either ENV32BIT or ENV64BIT"
#endif

	return result;
}


int
Random(
	const int	aFrom,
	const int	aTo)
{
	static std::random_device device;
	static std::mt19937 gen(device());
	
	std::uniform_int_distribution<> dis(aFrom, aTo);

	return dis(gen);
}
