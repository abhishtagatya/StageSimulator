#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageExtremityAnalysisParameter.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageExtremityAnalysisParameter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	float ExtremityScore = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	float Tolerance = 0.05f;

};