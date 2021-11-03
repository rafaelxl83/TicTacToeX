#pragma once
#include <iostream>

class Symbol
{
public:
	enum class AvailableSymbols : int
	{
							empty	= 255,
							X		= 1,
							O		= 3,
							M		= 6,
							S		= 10,
							H		= 15
	};
	struct SymbolProperty
	{
		AvailableSymbols	symbol;
		char				ico;
		int					value;
		int					victory;
	};
											Symbol(
												const Symbol&) = default;
											Symbol(
												AvailableSymbols			thePlayerSymbol);
											Symbol(
												short						thePlayerTurn);

	SymbolProperty							GetProperty();
											
	Symbol& operator=(const Symbol&) = default;
	bool operator==(const AvailableSymbols& s)
	{
		return this->myProperty.symbol == s;
	}
	bool operator==(const Symbol& symbol)
	{
		return this->myProperty.value == symbol.myProperty.value;
	}
	bool operator==(const int& value)
	{
		return this->myProperty.value == value;
	}

	friend std::ostream& operator<<(
		std::ostream& os,
		const Symbol& obj)
	{
		return os << obj.myProperty.ico
#ifdef _DEBUG
			<< "[" << obj.myProperty.value << "]"
#endif
			;
	}

private:
	union Union
	{
		AvailableSymbols	s;
		int					i;
	};

	void									SetProperty(
												Union						u, 
												char						ico);

	SymbolProperty							myProperty;
	char									AvailableIcons[6] = {' ','X','O','M','S','H'};
};

