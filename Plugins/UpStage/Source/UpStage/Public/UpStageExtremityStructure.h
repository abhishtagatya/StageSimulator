#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageExtremityStructure.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageExtremityStructure
{
	GENERATED_BODY()

	float MinValue = 0.f;
	float MaxValue = 0.f;
	bool bLookForPeak = true;
};