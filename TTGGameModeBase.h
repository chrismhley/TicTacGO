#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TTGGameModeBase.generated.h"

class UTTGBoardObject;
class UTTGSymbolData;
class UTTGSymbolObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTTGGameModeBase_StartGameEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTTGGameModeBase_StartRoundEvent, const FTimerHandle&, RoundTimer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTTGGameModeBase_GeneratedNextSymbolObjectEvent, const UTTGSymbolObject*, NextSymbolObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTTGGameModeBase_BoardUpdatedEvent, const int32, BoardIndex, const UTTGSymbolObject*, PlacedSymbolObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTTGGameModeBase_BoardResetEvent, const UTTGSymbolObject*, EmptySymbolObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTTGGameModeBase_MoveInvalidEvent, const int32, BoardIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTTGGameModeBase_CompletedLinesEvent, const TSet<int32>&, CompletedIndicesSet, const UTTGSymbolObject*, EmptySymbolObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTTGGameModeBase_OutOfMoves);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTTGGameModeBase_ScoreUpdatedEvent, const int32, CurrentScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTTGGameModeBase_EndRoundEvent);

UCLASS()
class TICTACGO_API ATTGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void CreateSymbolObjectsFromData(const UTTGSymbolData* SymbolData);

	/** Creates a game board of desired grid size. Be sure to call CreateSymbolObjectsFromData first to initialize the Empty Symbol Object. */
	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	UTTGBoardObject* CreateGameBoard(const int32 BoardSize);

public:
	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	const UTTGBoardObject* GetBoard() const;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	const UTTGSymbolObject* GetEmptySymbolObject() const;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	const float GetRoundDuration() const;

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	virtual void StartGame();

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	virtual void PlayAgain();

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	virtual void BoardCellClicked(const int32 CellIndex);

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_StartGameEvent OnStartGame;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_StartRoundEvent OnStartRound;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_GeneratedNextSymbolObjectEvent OnGeneratedNextSymbolObject;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_BoardUpdatedEvent OnBoardUpdated;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_BoardResetEvent OnBoardReset;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_MoveInvalidEvent OnMoveInvalid;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_CompletedLinesEvent OnCompletedLines;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_OutOfMoves OnOutOfMoves;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_ScoreUpdatedEvent OnScoreUpdated;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FTTGGameModeBase_EndRoundEvent OnEndRound;

	FTimerHandle RoundTimerHandle;

protected:
	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	virtual void StartRoundCountdown();
	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	virtual void StartRound();

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void GenerateNextSymbolObject();

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	bool IsMoveValid(const int32 BoardIndex) const;

	/** Makes a move by placing a symbol on the board. This should only be called after checking if the move is valid. */
	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void MakeMove(const int32 BoardIndex, const UTTGSymbolObject* SymbolObject);

	/** Checks for completed lines and marks the indices for reset to empty symbol. Returns the number of completed lines. */
	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	const int32 MarkCompletedLines();

	/** Marks a single index for reset to empty symbol. */
	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void MarkCompletedLineIndex(const int32 BoardRowIndex, const int32 BoardColumnIndex);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void HandleCompletedLines(const int32 NumCompletedLines);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void HandleOutOfMoves();

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void ResetBoard();

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void IncreaseScore(const int32 Points);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	void DecreaseScore(const int32 Points);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Gameplay")
	virtual void EndRound();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TicTacGo|Config")
	TObjectPtr<const UTTGSymbolData> SymbolDatabase;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacGo|Gameplay")
	TObjectPtr<UTTGSymbolObject> EmptySymbolObject;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacGo|Gameplay")
	TArray<TObjectPtr<const UTTGSymbolObject>> PlaceableSymbolObjects;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacGo|Gameplay")
	TObjectPtr<UTTGBoardObject> Board;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacGo|Gameplay")
	TObjectPtr<const UTTGSymbolObject> NextPlaceableSymbolObject;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacGo|Gameplay")
	bool bIsRoundStarted = false;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacGo|Gameplay")
	int32 Score = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TicTacGo|Config")
	float RoundDuration = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TicTacGo|Config")
	int32 PointsPerLine = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TicTacGo|Config")
	int32 MultiLineBonusPoints = 75;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TicTacGo|Config")
	int32 PointsLostForOutOfMoves = 125;

	UPROPERTY()
	TSet<int32> CompletedLinesIndicesSet;
};
