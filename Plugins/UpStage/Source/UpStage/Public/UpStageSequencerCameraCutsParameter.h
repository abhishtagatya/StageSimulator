#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageCameraSelectionParameter.h"
#include "UpStageCameraGenerationParameter.h"
#include "CineCameraActor.h"
#include "UpStageSequencerCameraCutsParameter.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageSequencerCameraCutsParameter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	FUpStageCameraSelectionParameter CameraSelectionParameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	FUpStageCameraGenerationParameter CameraGenerationParameter;
};