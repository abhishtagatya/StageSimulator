#pragma once

#include "CoreMinimal.h"
#include "UpStageLabanMovementAnalysisType.generated.h"


UENUM(BlueprintType)
enum class EUpStageLabanMovementAnalysisType : uint8
{
	Kinematic UMETA(DisplayName = "Kinematic"),
	Newtonian UMETA(DisplayName = "Newtonian")
};