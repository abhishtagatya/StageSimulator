#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageLabanEffortActionDimension.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageLabanEffortActionDimension
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effort")
	bool bIsIndirect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effort")
	bool bIsStrong;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effort")
	bool bIsSudden;
};