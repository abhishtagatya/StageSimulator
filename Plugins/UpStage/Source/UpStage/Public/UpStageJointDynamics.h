#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageJointDynamics.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageJointDynamics
{
	GENERATED_BODY()
	
	int32 NumFrames;
	TArray<FVector> Positions;
	TArray<FVector> Velocities;
	TArray<FVector> Accelerations;
	TArray<FVector> Jerks;
};