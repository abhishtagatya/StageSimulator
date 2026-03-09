#pragma once

#include "CoreMinimal.h"
#include "UpStageCameraCutFillMethod.generated.h"


UENUM(BlueprintType)
enum class EUpStageCameraCutFillMethod : uint8
{
	None UMETA(DisplayName = "None"),
	Fallback UMETA(DisplayName = "Fallback"),
	Neighbor UMETA(DisplayName = "Neighbor")
};