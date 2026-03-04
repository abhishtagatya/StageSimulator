#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageActionTransitionAnalysisParameter.h"
#include "UpStageExtremityAnalysisParameter.h"
#include "UpStageSequencerKeyMomentParameter.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageSequencerKeyMomentParameter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	int32 FramePadding = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis|Transition")
	bool bAnalyzeActionTransition = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis|Transition")
	FUpStageActionTransitionAnalysisParameter ActionTransitionAnalysisParameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis|Extremities")
	bool bAnalyzeExtremities = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis|Extremities")
	FUpStageExtremityAnalysisParameter ExtremityAnalysisParameter;
};