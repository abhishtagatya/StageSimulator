// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UpStagePerformerCalibrationAsset.generated.h"

// 1. Forward declare the classes here. 
class UAnimSequence;
class UPhysicsAsset;

/**
 * 
 */
UCLASS()
class UPSTAGE_API UUpStagePerformerCalibrationAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Baseline")
	TObjectPtr<UAnimSequence> BaselineAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity")
	float WeightSensitivity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity")
	float TimeSensitivity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proportions")
	TObjectPtr<UPhysicsAsset> PhysicsAsset;
};
