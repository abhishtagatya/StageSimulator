// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "UpStageLabanMovementAnalysisType.h"
#include "UpStageJointData.h"
#include "UpStageJointDynamics.h"
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
	
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

private:
	FTransform GetComponentSpaceTransform(UAnimSequence* AnimationSequence, FUpStageJointData TargetJoint, float Time) const;
	FUpStageJointDynamics CalculateJointDynamics(UAnimSequence* AnimationSequence, FUpStageJointData TargetJoint, TArray<float> Times) const;
	
	float CalculateEffortSpace(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const;
	float CalculateEffortWeight(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const;
	float CalculateEffortTime(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const;
};
