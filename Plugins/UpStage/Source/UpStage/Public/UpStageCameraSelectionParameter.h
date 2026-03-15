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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Visibility")
	bool bUseComplexTrace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Composition")
	float CompositionEdgeScreen = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Composition")
	float CompositionSafeScreen = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Composition")
	bool bCheckFocalMemberComposition = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Composition")
	float CompositionFocalMemberSafeBoost = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Composition")
	float CompositionOffScreenPenalty = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Composition")
	float CompositionEdgeScreenPenalty = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Composition")
	float CompositionRuleOfThirdsPortion = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection|Composition")
	float CompositionHeadroomPortion = 0.4f;
};