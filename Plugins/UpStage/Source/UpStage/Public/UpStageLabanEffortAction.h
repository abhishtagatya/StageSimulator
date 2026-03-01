#pragma once

#include "CoreMinimal.h"
#include "UpStageLabanEffortAction.generated.h"


UENUM(BlueprintType)
enum class EUpStageLabanEffortAction : uint8
{
	Float UMETA(DisplayName = "Float"),
	Punch UMETA(DisplayName = "Punch"),
	Glide UMETA(DisplayName = "Glide"),
	Slash UMETA(DisplayName = "Slash"),
	Dab   UMETA(DisplayName = "Dab"),
	Wring UMETA(DisplayName = "Wring"),
	Flick UMETA(DisplayName = "Flick"),
	Press UMETA(DisplayName = "Press"),
};