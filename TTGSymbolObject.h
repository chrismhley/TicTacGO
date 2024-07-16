#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TTGSymbolData.h"
#include "TTGSymbolObject.generated.h"

UCLASS()
class TICTACGO_API UTTGSymbolObject : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetParams(FName id, UTexture2D* icon, bool bIsEmptySymbol);

	UFUNCTION()
	FName GetSymbolID() const;

	UFUNCTION()
	UTexture2D* GetSymbolIcon() const;

	UFUNCTION()
	bool IsEmptySymbol() const;

private:
	UPROPERTY()
	FName SymbolID;

	UPROPERTY()
	FTTGSymbol Symbol;
};
