#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageCameraSelectionParameter.h"
#include "UpStageSequencerCameraCutsParameter.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageSequencerCameraCutsParameter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis|Selection")
	FUpStageCameraSelectionParameter CameraSelectionParameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Frame")
	int32 FrameSpan = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Frame")
	int32 PlaybackStart = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation|Frame")
	int32 PlaybackEnd = 0;
};