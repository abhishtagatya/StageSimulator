#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageLabanEffortAction.h"
#include "UpStageActionTransitionAnalysisParameter.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageActionTransitionAnalysisParameter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	int32 EffortTransitionScore = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	EUpStageLabanEffortAction BaseEffortAction = EUpStageLabanEffortAction::Glide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	FVector2D ActionTransitionThreshold = FVector2D(0.4f, 0.6f);
};