#include "TTGHUD.h"
#include <GameFramework/PlayerController.h>
#include <Blueprint/UserWidget.h>

ATTGHUD::ATTGHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ATTGHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenu();
}

void ATTGHUD::ShowMainMenu()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (!MainMenuScreen)
	{
		MainMenuScreen = CreateWidget<UUserWidget>(PlayerController, MainMenuClass);
	}
	MainMenuScreen->AddToViewport();
	PlayerController->bShowMouseCursor = true;
}

void ATTGHUD::ShowGameScreen()
{
	if (MainMenuScreen)
	{
		MainMenuScreen->RemoveFromParent();
		MainMenuScreen = nullptr;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (!GameScreen)
	{
		GameScreen = CreateWidget<UUserWidget>(PlayerController, GameScreenClass);
	}
	GameScreen->AddToViewport();
	PlayerController->bShowMouseCursor = true;
}
