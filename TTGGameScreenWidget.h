#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTGGameScreenWidget.generated.h"

class UBorder;
class UButton;
class UCanvasPanel;
class UImage;
class UOverlay;
class UTextBlock;
class UUniformGridPanel;
class UTTGCellWidget;
class UTTGSymbolObject;

UCLASS()
class TICTACGO_API UTTGGameScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void AddCellsToGridPanel();

	UFUNCTION()
	void UpdateRoundTimer(float TimeRemaining);

	UFUNCTION()
	void OnStartRound(const FTimerHandle& RoundTimer);

	UFUNCTION()
	void OnGeneratedNextSymbolObject(const UTTGSymbolObject* NextSymbolObject);

	UFUNCTION()
	void OnBoardUpdated(const int32 BoardIndex, const UTTGSymbolObject* PlacedSymbolObject);

	UFUNCTION()
	void OnBoardReset(const UTTGSymbolObject* EmptySymbolObject);

	UFUNCTION()
	void OnMoveInvalid(const int32 BoardIndex);

	UFUNCTION()
	void OnCompletedLines(const TSet<int32>& CompletedIndicesSet, const UTTGSymbolObject* EmptySymbolObject);

	UFUNCTION()
	void OnOutOfMoves();

	UFUNCTION()
	void OnScoreUpdated(const int32 Score);

	UFUNCTION()
	void OnEndRound();

	UFUNCTION()
	void OnPlayAgainButtonClicked();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ScreenCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "TicTacGo|HUD")
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "TicTacGo|HUD")
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "TicTacGo|HUD")
	TObjectPtr<UImage> NextSymbolImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "TicTacGo|HUD")
	TObjectPtr<UUniformGridPanel> CellGridPanel;

	UPROPERTY(EditDefaultsOnly, Category = "TicTacGo|HUD")
	TSubclassOf<UTTGCellWidget> CellWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UTTGCellWidget>> CellWidgets;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "TicTacGo|HUD")
	TObjectPtr<UTextBlock> FinalScoreText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "TicTacGo|HUD")
	TObjectPtr<UOverlay> PlayAgainOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "TicTacGo|HUD")
	TObjectPtr<UButton> PlayAgainButton;

	FTimerHandle RoundTimerHandle;

private:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> ScreenFadeIn;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> RoundEnded;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> NextRound;
};
