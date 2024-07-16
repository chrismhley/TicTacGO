#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TTGSymbolData.generated.h"

USTRUCT(BlueprintType)
struct FTTGSymbol
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TicTacGo")
	TObjectPtr<UTexture2D> SymbolIcon = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "TicTacGo")
	bool bIsEmptySymbol = false;
};

UCLASS(BlueprintType)
class TICTACGO_API UTTGSymbolData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TicTacGo")
	FTTGSymbol EmptySymbol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TicTacGo")
	TMap<FName, FTTGSymbol> SymbolMap;
};
