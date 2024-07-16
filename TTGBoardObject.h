#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TTGBoardObject.generated.h"

class UTTGSymbolObject;

UCLASS()
class TICTACGO_API UTTGBoardObject : public UObject
{
	GENERATED_BODY()
	
public:
	//@TODO: add support for non-uniform boards
	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	void InitBoard(const int32 UniformBoardSize, const UTTGSymbolObject* EmptySymbolObject);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	bool IsEmptyAtIndex(const int32 BoardIndex) const;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	void PlaceSymbolObject(const int32 BoardIndex, const UTTGSymbolObject* SymbolObject);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	void ResetBoard(const UTTGSymbolObject* EmptySymbolObject);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	bool IsMoveAvailable() const;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	const UTTGSymbolObject* GetSymbolObject(const int32 RowIndex, const int32 ColumnIndex) const;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	int32 GetNumRows() const;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	int32 GetNumColumns() const;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	int32 GetNumEmptyCells() const;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "TicTacGo")
	int32 NumRows;

	UPROPERTY(VisibleInstanceOnly, Category = "TicTacGo")
	int32 NumColumns;

	UPROPERTY(VisibleInstanceOnly, Category = "TicTacGo")
	int32 NumEmptyCells;

	UPROPERTY(VisibleInstanceOnly, Category = "TicTacGo")
	TArray<TObjectPtr<const UTTGSymbolObject>> SymbolObjectGrid;

	UPROPERTY(VisibleInstanceOnly, Category = "TicTacGo")
	FName EmptySymbolID;
};
