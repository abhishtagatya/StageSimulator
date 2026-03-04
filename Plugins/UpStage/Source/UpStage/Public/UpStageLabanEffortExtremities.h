#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageExtremityStructure.h"
#include "UpStageLabanEffortExtremities.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageLabanEffortExtremities
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extremity")
	FUpStageExtremityStructure SpaceExtremity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extremity")
	FUpStageExtremityStructure WeightExtremity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extremity")
	FUpStageExtremityStructure TimeExtremity;
};