#include "Symbol.h"

Symbol::Symbol(
	AvailableSymbols thePlayerSymbol)
{
	char ico;
	switch (thePlayerSymbol)
	{
	case AvailableSymbols::X:
		ico = 'X';
		break;
	case AvailableSymbols::O:
		ico = 'O';
		break;
	case AvailableSymbols::M:
		ico = 'M';
		break;
	case AvailableSymbols::S:
		ico = 'S';
		break;
	case AvailableSymbols::H:
		ico = 'H';
		break;
	default:
		ico = ' ';
		break;
	}

	Union u;
	u.s = thePlayerSymbol;
	SetProperty(u, ico);
}

Symbol::Symbol(
	unsigned short thePlayerTurn)
{
	Union u;
	char ico;
	switch (thePlayerTurn)
	{
	case 1:
		u.s = AvailableSymbols::X;
		ico = 'X';
		break;
	case 2:
		u.s = AvailableSymbols::O;
		ico = 'O';
		break;
	case 3:
		u.s = AvailableSymbols::M;
		ico = 'M';
		break;
	case 4:
		u.s = AvailableSymbols::S;
		ico = 'S';
		break;
	case 5:
		u.s = AvailableSymbols::H;
		ico = 'H';
		break;
	default:
		u.s = AvailableSymbols::empty;
		ico = ' ';
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
	Union u, 
	char ico)
{
	myProperty.ico = ico;
	myProperty.symbol = u.s;
	myProperty.value = u.i;
	myProperty.victory = 3 * u.i;
}
