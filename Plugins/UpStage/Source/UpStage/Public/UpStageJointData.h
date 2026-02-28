#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageJointData.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageJointData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joint")
	FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joint")
	float Weight = 1.f;

	TArray<FName> BonePathToRoot;
};