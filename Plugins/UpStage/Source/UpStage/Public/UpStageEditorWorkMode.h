#pragma once

#include "CoreMinimal.h"
#include "UpStageEditorWorkMode.generated.h"


UENUM(BlueprintType)
enum class EUpStageEditorWorkMode : uint8
{
	Complete UMETA(DisplayName = "Complete"),
	KeyMomentSelection UMETA(DisplayName = "Key Moment Selection"),
	CameraSelection UMETA(DisplayName = "Camera Selection"),
	QueueRenders UMETA(DisplayName = "Queue Renders"),
	Cancel UMETA(DisplayName = "Cancel") 
};