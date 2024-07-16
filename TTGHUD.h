#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TTGHUD.generated.h"

class UUserWidget;

UCLASS()
class TICTACGO_API ATTGHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATTGHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category = "TicTacGo")
	void ShowGameScreen();

	UPROPERTY(EditDefaultsOnly, Category = "TicTacGo")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MainMenuScreen;

	UPROPERTY(EditDefaultsOnly, Category = "TicTacGo")
	TSubclassOf<UUserWidget> GameScreenClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> GameScreen;
};
