#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageFrameMovementAnalysis.h"
#include "UpStagePerformerFocalStructure.h"
#include "UpStageSequencerMovementAnalysis.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageSequencerMovementAnalysis
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
	TArray<FUpStageFrameMovementAnalysis> FrameMovementAnalyses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	TArray<FUpStagePerformerFocalStructure> FrameFocalStructures;
};