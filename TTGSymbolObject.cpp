#include "TTGSymbolObject.h"

void UTTGSymbolObject::SetParams(FName id, UTexture2D* icon, bool bIsEmptySymbol)
{
	SymbolID = id;
	Symbol.SymbolIcon = icon;
	Symbol.bIsEmptySymbol = bIsEmptySymbol;
}

FName UTTGSymbolObject::GetSymbolID() const
{
	return SymbolID;
}

UTexture2D* UTTGSymbolObject::GetSymbolIcon() const
{
	return Symbol.SymbolIcon;
}

bool UTTGSymbolObject::IsEmptySymbol() const
{
	return Symbol.bIsEmptySymbol;
}
