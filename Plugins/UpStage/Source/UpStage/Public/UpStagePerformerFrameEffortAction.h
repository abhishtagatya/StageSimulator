#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageFrameMovementAnalysis.h"
#include "UpStagePerformerFrameEffortAction.generated.h"

USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStagePerformerFrameEffortAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performer", meta = (AllowedClasses = "UpStagePerformer"))
	TObjectPtr<AActor> Performer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	int32 FrameIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Analysis")
	FUpStageFrameMovementAnalysis FrameMovementAnalysis;
};