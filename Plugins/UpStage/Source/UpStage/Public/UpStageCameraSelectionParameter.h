#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UpStageCameraSelectionParameter.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageCameraSelectionParameter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	float AlignmentScore = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	float VisibilityScore = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	float CompositionScore = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	bool bUseComplexTrace = false;
};