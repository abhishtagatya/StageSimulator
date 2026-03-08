#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStagePerformerFocalMember.h"
#include "UpStagePerformerFocalStructure.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStagePerformerFocalStructure
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FTransform MainFocalTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	FVector MainFocalDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Members")
	TArray<FUpStagePerformerFocalMember> FocalMembers;
};