// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "UpStageLabanMovementAnalysisType.h"
#include "UpStageJointData.h"
#include "UpStageJointDynamics.h"
#include "UpStagePerformerCalibrationAsset.h"
#include "UpStageMathFunctions.h"
#include "UpStageLabanMovementModifier.generated.h"


/**
 * 
 */
UCLASS()
class UPSTAGE_API UUpStageLabanMovementModifier : public UAnimationModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<FUpStageJointData> Joints;

	UPROPERTY(EditAnywhere, Category = "Settings")
	EUpStageLabanMovementAnalysisType AnalysisType = EUpStageLabanMovementAnalysisType::Kinematic;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = "1"))
	int32 TemporalWindowSize = 27;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int32 ShortTimeWindowSize = 9;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int32 SmoothWindowSize = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MinSpaceDisplacement = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Calibration")
	bool bCalibrate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Calibration", meta = (EditCondition = "bCalibrate"))
	TObjectPtr<UUpStagePerformerCalibrationAsset> CalibrationAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Calibration", meta = (EditCondition = "bCalibrate"))
	bool bAsCalibrationBaseline = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Calibration", meta = (EditCondition = "bCalibrate"))
	bool bCreateNormalizeCurve = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Calibration", meta = (EditCondition = "bCalibrate"))
	float CalibrationHeadroom = 3.f;
	
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

private:
	FTransform GetComponentSpaceTransform(UAnimSequence* AnimationSequence, FUpStageJointData TargetJoint, float Time) const;
	FUpStageJointDynamics CalculateJointDynamics(UAnimSequence* AnimationSequence, FUpStageJointData TargetJoint, TArray<float> Times) const;
	
	float CalculateEffortSpace(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const;
	float CalculateEffortWeight(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const;
	float CalculateEffortTime(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const;

	TArray<FVector> SmoothVectorArray(const TArray<FVector>& Input, int32 WindowSize) const;
};
