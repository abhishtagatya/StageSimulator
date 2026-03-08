#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStagePerformerFocalMember.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStagePerformerFocalMember
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FTransform FocalMemberTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Importance")
	float ImportanceScore = 1.f;
};