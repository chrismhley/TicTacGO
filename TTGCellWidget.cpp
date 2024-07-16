#include "TTGCellWidget.h"
#include "TTGGameModeBase.h"
#include "TTGSymbolObject.h"
#include "Animation/WidgetAnimation.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include "Kismet/GameplayStatics.h"

void UTTGCellWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CellButton->OnClicked.AddUniqueDynamic(this, &UTTGCellWidget::OnCellButtonClicked);
	CellButton->OnHovered.AddUniqueDynamic(this, &UTTGCellWidget::OnCellButtonHovered);
	CellButton->OnUnhovered.AddUniqueDynamic(this, &UTTGCellWidget::OnCellButtonUnhovered);
}

void UTTGCellWidget::SetParams(const int32 BoardIndex, const UTTGSymbolObject* EmptySymbolObject)
{
	CellIndex = BoardIndex;
	SymbolObject = EmptySymbolObject;
	SymbolImage->SetBrushFromTexture(SymbolObject->GetSymbolIcon(), false);
}

void UTTGCellWidget::SetSymbolObject(const UTTGSymbolObject* NewSymbolObject, bool bIsCompletedLine)
{
	SymbolObject = NewSymbolObject;
	SymbolImage->SetBrushFromTexture(SymbolObject->GetSymbolIcon(), false);
	if (bIsCompletedLine)
	{
		PlayAnimation(CompletedLineAnim);
	}
	else
	{
		if (SymbolObject->IsEmptySymbol())
		{
			PlayAnimation(SymbolEmptiedAnim);
		}
		else
		{
			PlayAnimation(SymbolPlacedAnim);
		}
	}
	OnSymbolSet(SymbolObject);
}

void UTTGCellWidget::PlayOutOfMovesAnimation()
{
	PlayAnimation(OutOfMovesAnim);
}

void UTTGCellWidget::PlayMoveInvalidAnimation()
{
	PlayAnimation(MoveInvalidAnim);
}

void UTTGCellWidget::OnCellButtonClicked()
{
	CastChecked<ATTGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->BoardCellClicked(CellIndex);
}

void UTTGCellWidget::OnCellButtonHovered()
{
	PlayAnimationForward(HoveredAnim);
}

void UTTGCellWidget::OnCellButtonUnhovered()
{
	PlayAnimationReverse(HoveredAnim);
}
