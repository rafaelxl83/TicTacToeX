#pragma once
#include <iostream>

class Symbol
{
public:
	enum class AvailableSymbols : int
	{
							empty	= 0,
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
												AvailableSymbols			thePlayerSymbol);
											Symbol(
												unsigned short				thePlayerTurn);

	SymbolProperty							GetProperty();

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

	SymbolProperty			myProperty;

};

