#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageCameraCutSpecification.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageCameraCutSpecification
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float FocusDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FQuat CameraRotation = FQuat::Identity;
};