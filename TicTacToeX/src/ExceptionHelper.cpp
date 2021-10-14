#include "ExceptionHelper.h"

std::string
ExceptionHelper::what(
	const std::exception_ptr&					eptr)
{
	if (!eptr) { throw std::bad_exception(); }

	try { std::rethrow_exception(eptr); }
	catch (const std::exception &e) { return e.what() + nested_what(e); }
	catch (const std::string    &e) { return e; }
	catch (const char           *e) { return e; }
	catch (...) { return "Unknown exception"; }
}