#pragma once

struct SymbolProperty
{
	char		symbol;
	int			value;
	int			victory;
};

class Symbol
{
public:
	Symbol(
		unsigned short thePlayer)
	{
		Union u;
		switch (thePlayer)
		{
			case 2:
				u.s = AvailableSymbols::O;
				myProperty.symbol = 'O';
				break;
			case 3:
				u.s = AvailableSymbols::M;
				myProperty.symbol = 'M';
				break;
			case 4:
				u.s = AvailableSymbols::S;
				myProperty.symbol = 'S';
				break;
			case 5:
				u.s = AvailableSymbols::H;
				myProperty.symbol = 'H';
				break;
			default:
				u.s = AvailableSymbols::X;
				myProperty.symbol = 'X';
				break;
		}

		myProperty.value = u.i;
		myProperty.victory = 3 * u.i;
	}

	SymbolProperty GetProperty()
	{
		return myProperty;
	}

	friend std::ostream& operator<<(
		std::ostream& os, 
		const Symbol& obj)
	{
		return os << obj.myProperty.symbol
#ifdef _DEBUG
			<< "[" << obj.myProperty.value << "]"
#endif
			;
	}

	friend std::istream& operator<<(
		std::istream& is, 
		const Symbol& obj)
	{
		return is;
	}

private:
	enum class AvailableSymbols : int
	{
		X = 1,
		O = 3,
		M = 6,
		S = 10,
		H = 15
	};
	union Union
	{
		AvailableSymbols	s;
		int					i;
	};

	SymbolProperty			myProperty;

};

