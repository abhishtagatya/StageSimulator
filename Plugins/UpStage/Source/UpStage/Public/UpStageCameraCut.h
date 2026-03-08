#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageCameraCut.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageCameraCut
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	ACineCameraActor* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	int32 FrameIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	int32 FrameStart = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
	int32 FrameEnd = 0;
};