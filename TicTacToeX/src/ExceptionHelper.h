#pragma once

#include <exception>
#include <stdexcept>

class ExceptionHelper
{
public:

	static std::string						what(
												const std::exception_ptr&	eptr = std::current_exception());

	template <typename T>
	static std::string						nested_what(
												const T&					e)
	{
		try { std::rethrow_if_nested(e); }
		catch (...) { return " (" + what(std::current_exception()) + ")"; }
		return {};
	}
};
