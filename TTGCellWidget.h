#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTGCellWidget.generated.h"

class UButton;
class UImage;
class UTTGSymbolObject;

UCLASS()
class TICTACGO_API UTTGCellWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Cell")
	void SetParams(const int32 BoardIndex, const UTTGSymbolObject* EmptySymbolObject);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Cell")
	void SetSymbolObject(const UTTGSymbolObject* NewSymbolObject, bool bIsCompletedLine = false);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSymbolSet(const UTTGSymbolObject* NewSymbolObject);

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Cell")
	void PlayOutOfMovesAnimation();

	UFUNCTION(BlueprintCallable, Category = "TicTacGo|Cell")
	void PlayMoveInvalidAnimation();

protected:
	UFUNCTION()
	void OnCellButtonClicked();

	UFUNCTION()
	void OnCellButtonHovered();

	UFUNCTION()
	void OnCellButtonUnhovered();

	UPROPERTY(BlueprintReadOnly)
	int32 CellIndex;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UTTGSymbolObject> SymbolObject;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CellButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> SymbolImage;

private:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> OutOfMovesAnim;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> MoveInvalidAnim;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> HoveredAnim;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> SymbolPlacedAnim;

	/** This is called when the cell gets emptied without completing a line. */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> SymbolEmptiedAnim;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> CompletedLineAnim;
};
