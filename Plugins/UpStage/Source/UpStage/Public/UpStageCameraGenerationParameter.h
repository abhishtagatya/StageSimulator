#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageCameraCutFillMethod.h"
#include "CineCameraActor.h"
#include "UpStageCameraGenerationParameter.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageCameraGenerationParameter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 FrameSpan = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int32 MinFrameDistance = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	bool bOverlapCuts = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	EUpStageCameraCutFillMethod CutFillMethod = EUpStageCameraCutFillMethod::Fallback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	ACineCameraActor* FallbackCamera;
};