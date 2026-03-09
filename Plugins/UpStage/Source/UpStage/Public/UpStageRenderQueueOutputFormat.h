#pragma once

#include "CoreMinimal.h"
#include "UpStageRenderQueueOutputFormat.generated.h"


UENUM(BlueprintType)
enum class EUpStageRenderQueueOutputFormat : uint8
{
	StillsJPG UMETA(DisplayName = "Stills as JPG"),
	StillsPNG UMETA(DisplayName = "Stills as PNG"),
	ClipsMP4 UMETA(DisplayName = "Clips as MP4")
};