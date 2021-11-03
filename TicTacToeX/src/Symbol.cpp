#include "Symbol.h"

Symbol::Symbol(
	AvailableSymbols			thePlayerSymbol)
{
	Union u;
	char ico;
	switch (thePlayerSymbol)
	{
	case AvailableSymbols::X:
		ico = AvailableIcons[1];
		break;
	case AvailableSymbols::O:
		ico = AvailableIcons[2];
		break;
	case AvailableSymbols::M:
		ico = AvailableIcons[3];
		break;
	case AvailableSymbols::S:
		ico = AvailableIcons[4];
		break;
	case AvailableSymbols::H:
		ico = AvailableIcons[5];
		break;
	default:
		ico = AvailableIcons[0];
		break;
	}

	u.s = thePlayerSymbol;
	SetProperty(u, ico);
}

Symbol::Symbol(
	short						thePlayerTurn)
{
	Union u;
	char ico;
	switch (thePlayerTurn)
	{
	case 1:
		u.s = AvailableSymbols::X;
		ico = AvailableIcons[1];
		break;
	case 2:
		u.s = AvailableSymbols::O;
		ico = AvailableIcons[2];
		break;
	case 3:
		u.s = AvailableSymbols::M;
		ico = AvailableIcons[3];
		break;
	case 4:
		u.s = AvailableSymbols::S;
		ico = AvailableIcons[4];
		break;
	case 5:
		u.s = AvailableSymbols::H;
		ico = AvailableIcons[5];
		break;
	default:
		u.s = AvailableSymbols::empty;
		ico = AvailableIcons[0];
		break;
	}

	SetProperty(u, ico);
}

Symbol::SymbolProperty
Symbol::GetProperty()
{
	return myProperty;
}

void 
Symbol::SetProperty(
	Union						u, 
	char						ico)
{
	myProperty.ico		 = ico;
	myProperty.symbol	 = u.s;
	myProperty.value	 = u.i;
	myProperty.victory	 = 3 * u.i;
}
