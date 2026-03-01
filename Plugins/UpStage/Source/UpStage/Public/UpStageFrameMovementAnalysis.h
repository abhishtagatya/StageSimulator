#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageLabanEffortAction.h"
#include "UpStageFrameMovementAnalysis.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageFrameMovementAnalysis
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laban Effort")
	EUpStageLabanEffortAction EffortAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laban Effort")
	float EffortSpace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laban Effort")
	float EffortWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laban Effort")
	float EffortTime;
};