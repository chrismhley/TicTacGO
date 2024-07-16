#include "TTGMainMenuWidget.h"
#include "TTGGameModeBase.h"
#include "Animation/WidgetAnimation.h"
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include "Kismet/GameplayStatics.h"

void UTTGMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnHovered.AddUniqueDynamic(this, &UTTGMainMenuWidget::OnStartButtonHovered);
	StartButton->OnUnhovered.AddUniqueDynamic(this, &UTTGMainMenuWidget::OnStartButtonUnhovered);
	StartButton->OnPressed.AddUniqueDynamic(this, &UTTGMainMenuWidget::OnStartButtonPressed);
	StartButton->OnReleased.AddUniqueDynamic(this, &UTTGMainMenuWidget::OnStartButtonReleased);

	// Ensure the screen's intended starting state is set
	ScreenCanvas->SetRenderOpacity(0.0f);
	PlayAnimationForward(ScreenFadeIn);
}

void UTTGMainMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (ScreenFadeOut == Animation)
	{
		CastChecked<ATTGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->StartGame();
	}
}

void UTTGMainMenuWidget::OnStartButtonHovered()
{
	if (!StartButton->IsPressed())
	{
		PlayAnimationForward(StartButtonHovered);
	}
}

void UTTGMainMenuWidget::OnStartButtonUnhovered()
{
	if (!StartButton->IsPressed())
	{
		PlayAnimationReverse(StartButtonHovered);
	}
}

void UTTGMainMenuWidget::OnStartButtonPressed()
{
	PlayAnimationForward(StartButtonPressed);
}

void UTTGMainMenuWidget::OnStartButtonReleased()
{
	PlayAnimationReverse(StartButtonPressed);
	PlayAnimationForward(ScreenFadeOut);
	StartButton->SetIsEnabled(false);
}
