#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageFrameMovementAnalysis.h"
#include "UpStageSequencerMovementAnalysis.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageSequencerMovementAnalysis
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<FUpStageFrameMovementAnalysis> FrameMovementAnalyses;
};