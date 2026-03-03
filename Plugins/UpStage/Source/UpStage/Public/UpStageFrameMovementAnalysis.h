#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageLabanEffortAction.h"
#include "UpStageFrameMovementAnalysis.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageFrameMovementAnalysis
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effort")
	float EffortSpace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effort")
	float EffortWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effort")
	float EffortTime;
};