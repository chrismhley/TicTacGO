#include "TTGBoardObject.h"
#include "TTGSymbolObject.h"

void UTTGBoardObject::InitBoard(const int32 UniformBoardSize, const UTTGSymbolObject* EmptySymbolObject)
{
	for (int32 Index = 0; Index < UniformBoardSize * UniformBoardSize; ++Index)
	{
		SymbolObjectGrid.Add(EmptySymbolObject);
	}
	NumRows = UniformBoardSize;
	NumColumns = UniformBoardSize;
	NumEmptyCells = SymbolObjectGrid.Num();
	EmptySymbolID = EmptySymbolObject->GetSymbolID();
}

bool UTTGBoardObject::IsEmptyAtIndex(const int32 BoardIndex) const
{
	if (SymbolObjectGrid.IsValidIndex(BoardIndex))
	{
		return SymbolObjectGrid[BoardIndex]->GetSymbolID() == EmptySymbolID;
	}
	return false;
}

void UTTGBoardObject::PlaceSymbolObject(const int32 BoardIndex, const UTTGSymbolObject* SymbolObject)
{
	if (ensureMsgf(SymbolObject != nullptr, TEXT("Symbol Object being placed does not exist!")))
	{
		SymbolObjectGrid[BoardIndex] = SymbolObject;
		NumEmptyCells += SymbolObject->IsEmptySymbol() ? 1 : -1;
	}
}

void UTTGBoardObject::ResetBoard(const UTTGSymbolObject* EmptySymbolObject)
{
	for (int32 i = 0; i < SymbolObjectGrid.Num(); ++i)
	{
		SymbolObjectGrid[i] = EmptySymbolObject;
	}
	NumEmptyCells = SymbolObjectGrid.Num();
}

bool UTTGBoardObject::IsMoveAvailable() const
{
	return NumEmptyCells > 0;
}

const UTTGSymbolObject* UTTGBoardObject::GetSymbolObject(const int32 RowIndex, const int32 ColumnIndex) const
{
	return SymbolObjectGrid[RowIndex * NumRows + ColumnIndex];
}

int32 UTTGBoardObject::GetNumRows() const
{
	return NumRows;
}

int32 UTTGBoardObject::GetNumColumns() const
{
	return NumColumns;
}

int32 UTTGBoardObject::GetNumEmptyCells() const
{
	return NumEmptyCells;
}
