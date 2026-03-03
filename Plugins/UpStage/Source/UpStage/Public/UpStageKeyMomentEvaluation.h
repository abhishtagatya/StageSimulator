#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageKeyMomentEvaluation.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageKeyMomentEvaluation
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	TArray<AActor*> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	int32 FrameIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 KeyMomentScore;
};