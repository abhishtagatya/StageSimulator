#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStagePerformerFocalStructure.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStagePerformerFocalStructure
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FTransform FocalTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	FVector FocalForwardDirection;
};