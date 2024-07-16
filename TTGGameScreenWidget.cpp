#include "TTGGameScreenWidget.h"
#include "TTGBoardObject.h"
#include "TTGCellWidget.h"
#include "TTGGameModeBase.h"
#include "TTGSymbolObject.h"
#include "Animation/WidgetAnimation.h"
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>
#include <Components/Overlay.h>
#include <Components/TextBlock.h>
#include <Components/UniformGridPanel.h>
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UTTGGameScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AddCellsToGridPanel();
	
	ATTGGameModeBase* GameModeBase = CastChecked<ATTGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameModeBase->OnStartRound.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnStartRound);
	GameModeBase->OnGeneratedNextSymbolObject.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnGeneratedNextSymbolObject);
	GameModeBase->OnBoardUpdated.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnBoardUpdated);
	GameModeBase->OnBoardReset.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnBoardReset);
	GameModeBase->OnMoveInvalid.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnMoveInvalid);
	GameModeBase->OnCompletedLines.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnCompletedLines);
	GameModeBase->OnOutOfMoves.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnOutOfMoves);
	GameModeBase->OnScoreUpdated.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnScoreUpdated);
	GameModeBase->OnEndRound.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnEndRound);

	PlayAgainOverlay->SetVisibility(ESlateVisibility::Hidden);
	PlayAgainButton->OnClicked.AddUniqueDynamic(this, &UTTGGameScreenWidget::OnPlayAgainButtonClicked);

	// Ensure the screen's intended starting state is set
	ScreenCanvas->SetRenderOpacity(0.0f);
	UpdateRoundTimer(GameModeBase->GetRoundDuration());
	OnScoreUpdated(0);
	PlayAnimationForward(ScreenFadeIn);
}

void UTTGGameScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (RoundTimerHandle.IsValid())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(RoundTimerHandle))
		{
			UpdateRoundTimer(GetWorld()->GetTimerManager().GetTimerRemaining(RoundTimerHandle));
		}
	}
}

void UTTGGameScreenWidget::AddCellsToGridPanel()
{
	CellGridPanel->ClearChildren();
	CellWidgets.Empty();

	if (ATTGGameModeBase* GameMode = CastChecked<ATTGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		for (int32 Row = 0; Row < GameMode->GetBoard()->GetNumRows(); ++Row)
		{
			for (int32 Col = 0; Col < GameMode->GetBoard()->GetNumColumns(); ++Col)
			{
				UTTGCellWidget* CellWidget = CreateWidget<UTTGCellWidget>(this, CellWidgetClass);
				CellWidget->SetParams(Row * GameMode->GetBoard()->GetNumRows() + Col, GameMode->GetEmptySymbolObject());

				CellGridPanel->AddChildToUniformGrid(CellWidget, Row, Col);
				CellWidgets.Add(CellWidget);
			}
		}
	}
}

void UTTGGameScreenWidget::UpdateRoundTimer(float TimeRemaining)
{
	//@TODO: play timer counting down animation with 2 decimal places
	TimerText->SetText(UKismetTextLibrary::Conv_IntToText(UKismetMathLibrary::FCeil(TimeRemaining), false, false));
}

void UTTGGameScreenWidget::OnStartRound(const FTimerHandle& RoundTimer)
{
	RoundTimerHandle = RoundTimer;
	UpdateRoundTimer(GetWorld()->GetTimerManager().GetTimerRemaining(RoundTimerHandle));
}

void UTTGGameScreenWidget::OnGeneratedNextSymbolObject(const UTTGSymbolObject* NextSymbolObject)
{
	NextSymbolImage->SetBrushFromTexture(NextSymbolObject->GetSymbolIcon(), false);
}

void UTTGGameScreenWidget::OnBoardUpdated(const int32 BoardIndex, const UTTGSymbolObject* PlacedSymbolObject)
{
	if (ensureMsgf(CellWidgets.IsValidIndex(BoardIndex), TEXT("The list of Cell Widgets does not correctly represent the Board! Fix this!")))
	{
		CellWidgets[BoardIndex]->SetSymbolObject(PlacedSymbolObject);
	}
}

void UTTGGameScreenWidget::OnBoardReset(const UTTGSymbolObject* EmptySymbolObject)
{
	for (UTTGCellWidget* CellWidget : CellWidgets)
	{
		CellWidget->SetSymbolObject(EmptySymbolObject, false);
	}
}

void UTTGGameScreenWidget::OnMoveInvalid(const int32 BoardIndex)
{
	if (ensureMsgf(CellWidgets.IsValidIndex(BoardIndex), TEXT("The list of Cell Widgets does not correctly represent the Board! Fix this!")))
	{
		CellWidgets[BoardIndex]->PlayMoveInvalidAnimation();
	}
}

void UTTGGameScreenWidget::OnCompletedLines(const TSet<int32>& CompletedIndicesSet, const UTTGSymbolObject* EmptySymbolObject)
{
	for (int32 BoardIndex : CompletedIndicesSet)
	{
		if (ensureMsgf(CellWidgets.IsValidIndex(BoardIndex), TEXT("The list of Cell Widgets does not correctly represent the Board! Fix this!")))
		{
			CellWidgets[BoardIndex]->SetSymbolObject(EmptySymbolObject, true);
		}
	}
}

void UTTGGameScreenWidget::OnOutOfMoves()
{
	for (UTTGCellWidget* CellWidget : CellWidgets)
	{
		CellWidget->PlayOutOfMovesAnimation();
	}
}

void UTTGGameScreenWidget::OnScoreUpdated(const int32 Score)
{
	ScoreText->SetText(UKismetTextLibrary::Conv_IntToText(Score, false, false));
}

void UTTGGameScreenWidget::OnEndRound()
{
	UpdateRoundTimer(0.0f);
	//@TODO: play counting up animation on final score text
	FinalScoreText->SetText(ScoreText->GetText());
	PlayAnimation(RoundEnded);
}

void UTTGGameScreenWidget::OnPlayAgainButtonClicked()
{
	CastChecked<ATTGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->PlayAgain();
	PlayAnimation(NextRound);
}
