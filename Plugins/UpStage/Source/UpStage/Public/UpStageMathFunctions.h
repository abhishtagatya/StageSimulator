// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpStageFrameMovementAnalysis.h"
#include "UpStageLabanEffortAction.h"
#include "UpStageMathFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UPSTAGE_API UUpStageMathFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UpStage|Math")
	static float Normalize(const float X, const float K);
	
	UFUNCTION(BlueprintCallable, Category = "UpStage|Math")
	static float InverseRatio(const float X);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Selection")
	static EUpStageLabanEffortAction SelectEffortAction(FUpStageFrameMovementAnalysis FrameMovementAnalysis, FVector2D Threshold);
};
