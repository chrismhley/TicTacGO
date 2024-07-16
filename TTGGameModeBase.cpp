#include "TTGGameModeBase.h"
#include "TTGBoardObject.h"
#include "TTGHUD.h"
#include "TTGSymbolData.h"
#include "TTGSymbolObject.h"
#include <GameFramework/PlayerController.h>

void ATTGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CreateSymbolObjectsFromData(SymbolDatabase);

	//@TODO: make board size configurable and add support for non-uniform 3x3 boards
	Board = CreateGameBoard(3);
}

void ATTGGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(RoundTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void ATTGGameModeBase::CreateSymbolObjectsFromData(const UTTGSymbolData* SymbolData)
{
	if (SymbolData != nullptr)
	{
		// Create the empty symbol
		EmptySymbolObject = NewObject<UTTGSymbolObject>();//SymbolData->EmptySymbol;
		EmptySymbolObject->SetParams(FName(TEXT("Empty")), SymbolData->EmptySymbol.SymbolIcon, true);

		// Create the placeable symbols
		for (const TPair<FName, FTTGSymbol>& Pair : SymbolData->SymbolMap)
		{
			UTTGSymbolObject* SymbolObject = NewObject<UTTGSymbolObject>();
			SymbolObject->SetParams(Pair.Key, Pair.Value.SymbolIcon, false);
			PlaceableSymbolObjects.Add(SymbolObject);
		}
	}
}

UTTGBoardObject* ATTGGameModeBase::CreateGameBoard(const int32 BoardSize)
{
	if (ensureMsgf(EmptySymbolObject != nullptr, TEXT("Empty Symbol Object is not set, which is required for the game's Board to initialize!")))
	{
		UTTGBoardObject* GameBoard = NewObject<UTTGBoardObject>();
		GameBoard->InitBoard(BoardSize, EmptySymbolObject);
		return GameBoard;
	}
	return nullptr;
}

const UTTGBoardObject* ATTGGameModeBase::GetBoard() const
{
	return Board;
}

const UTTGSymbolObject* ATTGGameModeBase::GetEmptySymbolObject() const
{
	return EmptySymbolObject;
}

const float ATTGGameModeBase::GetRoundDuration() const
{
	return RoundDuration;
}

void ATTGGameModeBase::StartGame()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
	if (ATTGHUD* Hud = CastChecked<ATTGHUD>(PlayerController->GetHUD(), ECastCheckedType::NullAllowed))
	{
		Hud->ShowGameScreen();
		//@TODO: implement round countdown
		StartRound();
	}
}

void ATTGGameModeBase::PlayAgain()
{
	ResetBoard();
	StartRound();
}

void ATTGGameModeBase::BoardCellClicked(const int32 CellIndex)
{
	if (bIsRoundStarted)
	{
		if (IsMoveValid(CellIndex))
		{
			MakeMove(CellIndex, NextPlaceableSymbolObject);
			GenerateNextSymbolObject();
			const int32 NumCompletedLines = MarkCompletedLines();
			if (NumCompletedLines > 0)
			{
				HandleCompletedLines(NumCompletedLines);
			}
			else
			{
				if (!Board->IsMoveAvailable())
				{
					HandleOutOfMoves();
					ResetBoard();
				}
			}
		}
		else
		{
			OnMoveInvalid.Broadcast(CellIndex);
		}
	}
}

void ATTGGameModeBase::StartRoundCountdown()
{
	//@TODO
}

void ATTGGameModeBase::StartRound()
{
	bIsRoundStarted = true;
	Score = 0;
	OnScoreUpdated.Broadcast(Score);
	GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &ATTGGameModeBase::EndRound, RoundDuration);
	OnStartRound.Broadcast(RoundTimerHandle);
	GenerateNextSymbolObject();
}

void ATTGGameModeBase::GenerateNextSymbolObject()
{
	if (PlaceableSymbolObjects.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PlaceableSymbolObjects.Num() - 1);
		NextPlaceableSymbolObject = PlaceableSymbolObjects[RandomIndex];
		OnGeneratedNextSymbolObject.Broadcast(NextPlaceableSymbolObject);
	}
}

bool ATTGGameModeBase::IsMoveValid(const int32 BoardIndex) const
{
	return Board->IsEmptyAtIndex(BoardIndex);
}

void ATTGGameModeBase::MakeMove(const int32 BoardIndex, const UTTGSymbolObject* SymbolObject)
{
	Board->PlaceSymbolObject(BoardIndex, SymbolObject);
	OnBoardUpdated.Broadcast(BoardIndex, SymbolObject);
}

const int32 ATTGGameModeBase::MarkCompletedLines()
{
	//@TODO: add support for non-uniform 3x3 boards
	int32 NumCompletedLines = 0;

	// Check for completed rows
	for (int32 i = 0; i < Board->GetNumRows(); ++i)
	{
		if (Board->GetSymbolObject(i, 0)->GetSymbolID() != EmptySymbolObject->GetSymbolID()
			&& Board->GetSymbolObject(i, 0)->GetSymbolID() == Board->GetSymbolObject(i, 1)->GetSymbolID()
			&& Board->GetSymbolObject(i, 1)->GetSymbolID() == Board->GetSymbolObject(i, 2)->GetSymbolID())
		{
			MarkCompletedLineIndex(i, 0);
			MarkCompletedLineIndex(i, 1);
			MarkCompletedLineIndex(i, 2);
			++NumCompletedLines;
		}
	}

	// Check for completed columns
	for (int32 i = 0; i < Board->GetNumColumns(); ++i)
	{
		if (Board->GetSymbolObject(0, i)->GetSymbolID() != EmptySymbolObject->GetSymbolID()
			&& Board->GetSymbolObject(0, i)->GetSymbolID() == Board->GetSymbolObject(1, i)->GetSymbolID()
			&& Board->GetSymbolObject(1, i)->GetSymbolID() == Board->GetSymbolObject(2, i)->GetSymbolID())
		{
			MarkCompletedLineIndex(0, i);
			MarkCompletedLineIndex(1, i);
			MarkCompletedLineIndex(2, i);
			++NumCompletedLines;
		}
	}

	// Check for completed diagonal
	if (Board->GetSymbolObject(0, 0)->GetSymbolID() != EmptySymbolObject->GetSymbolID()
		&& Board->GetSymbolObject(0, 0)->GetSymbolID() == Board->GetSymbolObject(1, 1)->GetSymbolID()
		&& Board->GetSymbolObject(1, 1)->GetSymbolID() == Board->GetSymbolObject(2, 2)->GetSymbolID())
	{
		MarkCompletedLineIndex(0, 0);
		MarkCompletedLineIndex(1, 1);
		MarkCompletedLineIndex(2, 2);
		++NumCompletedLines;
	}

	// Check for completed anti-diagonal
	if (Board->GetSymbolObject(0, 2)->GetSymbolID() != EmptySymbolObject->GetSymbolID()
		&& Board->GetSymbolObject(0, 2)->GetSymbolID() == Board->GetSymbolObject(1, 1)->GetSymbolID()
		&& Board->GetSymbolObject(1, 1)->GetSymbolID() == Board->GetSymbolObject(2, 0)->GetSymbolID())
	{
		MarkCompletedLineIndex(0, 2);
		MarkCompletedLineIndex(1, 1);
		MarkCompletedLineIndex(2, 0);
		++NumCompletedLines;
	}

	return NumCompletedLines;
}

void ATTGGameModeBase::MarkCompletedLineIndex(const int32 BoardRowIndex, const int32 BoardColumnIndex)
{
	CompletedLinesIndicesSet.Add(BoardRowIndex * Board->GetNumRows() + BoardColumnIndex);
}

void ATTGGameModeBase::HandleCompletedLines(const int32 NumCompletedLines)
{
	// Increase score based on number of completed lines
	IncreaseScore(PointsPerLine * NumCompletedLines + (NumCompletedLines > 1 ? MultiLineBonusPoints : 0));

	// Update board
	for (int32 BoardIndex : CompletedLinesIndicesSet)
	{
		Board->PlaceSymbolObject(BoardIndex, EmptySymbolObject);
	}
	OnCompletedLines.Broadcast(CompletedLinesIndicesSet, EmptySymbolObject);
	CompletedLinesIndicesSet.Empty(CompletedLinesIndicesSet.Num());
}

void ATTGGameModeBase::HandleOutOfMoves()
{
	DecreaseScore(PointsLostForOutOfMoves);
	OnOutOfMoves.Broadcast();
}

void ATTGGameModeBase::ResetBoard()
{
	Board->ResetBoard(EmptySymbolObject);
	OnBoardReset.Broadcast(EmptySymbolObject);
}

void ATTGGameModeBase::IncreaseScore(const int32 Points)
{
	Score += Points;
	OnScoreUpdated.Broadcast(Score);
}

void ATTGGameModeBase::DecreaseScore(const int32 Points)
{
	Score = FMath::Max(Score - Points, 0);
	OnScoreUpdated.Broadcast(Score);
}

void ATTGGameModeBase::EndRound()
{
	bIsRoundStarted = false;
	GetWorldTimerManager().ClearTimer(RoundTimerHandle);
	OnEndRound.Broadcast();
}
