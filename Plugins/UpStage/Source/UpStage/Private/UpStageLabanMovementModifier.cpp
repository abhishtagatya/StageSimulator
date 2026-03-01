// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageLabanMovementModifier.h"
#include "UpStageLabanMovementAnalysisType.h"
#include "UpStageJointData.h"
#include "UpStageJointDynamics.h"
#include "UpStagePerformerCalibrationAsset.h"
#include "UpStageMathFunctions.h"

void UUpStageLabanMovementModifier::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	if (!AnimationSequence) return;
	if (Joints.Num() == 0) return;

	int32 NumFrames = 0;
	UAnimationBlueprintLibrary::GetNumFrames(AnimationSequence, NumFrames);
	if (ShortTimeWindowSize >= NumFrames || TemporalWindowSize >= NumFrames) return;
	if (ShortTimeWindowSize >= TemporalWindowSize) return;

	OnRevert_Implementation(AnimationSequence);

	// Precompute times for each frame
	TArray<float> Times;
	Times.Reserve(NumFrames);
	for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
	{
		float Time = 0.f;
		UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence, FrameIndex, Time);
		Times.Add(Time);
	}

	TArray<float> EffortSpace;
	TArray<float> EffortWeight;
	TArray<float> EffortTime;

	EffortSpace.Init(0.f, NumFrames);
	EffortWeight.Init(0.f, NumFrames);
	EffortTime.Init(0.f, NumFrames);

	for (int i = 0; i < Joints.Num(); ++i)
	{
		if (Joints[i].BoneName == NAME_None) continue;
		if (Joints[i].Weight <= 0.f) continue;

		float JointWeight = Joints[i].Weight;

		FUpStageJointDynamics JointDynamics = CalculateJointDynamics(AnimationSequence, Joints[i], Times);

		for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
		{
			EffortSpace[FrameIndex] += JointWeight * CalculateEffortSpace(JointDynamics, FrameIndex, NumFrames);
			EffortWeight[FrameIndex] += JointWeight * CalculateEffortWeight(JointDynamics, FrameIndex, NumFrames);
			EffortTime[FrameIndex] += JointWeight * CalculateEffortTime(JointDynamics, FrameIndex, NumFrames);
		}
	}

	FName EffortSpaceCurveName = TEXT("LabanEffortSpace");
	FName EffortWeightCurveName = TEXT("LabanEffortWeight");
	FName EffortTimeCurveName = TEXT("LabanEffortTime");

	UAnimationBlueprintLibrary::AddCurve(AnimationSequence, EffortSpaceCurveName, ERawCurveTrackTypes::RCT_Float);
	UAnimationBlueprintLibrary::AddCurve(AnimationSequence, EffortWeightCurveName, ERawCurveTrackTypes::RCT_Float);
	UAnimationBlueprintLibrary::AddCurve(AnimationSequence, EffortTimeCurveName, ERawCurveTrackTypes::RCT_Float);

	float TotalWeight = 0.f;
	float TotalTime = 0.f;

	// TODO: Fix the missing end frame data due to temporal window. Potential solution: Mirror the last valid value for the remaining frames / get average.
	for (int i = 0; i < Times.Num(); ++i)
	{
		UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, EffortSpaceCurveName, Times[i], EffortSpace[i]);
		UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, EffortWeightCurveName, Times[i], EffortWeight[i]);
		UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, EffortTimeCurveName, Times[i], EffortTime[i]);

		if (bCalibrate)
		{
			TotalWeight += EffortWeight[i];
			TotalTime += EffortTime[i];
		}
	}

	if (bCalibrate && CalibrationAsset)
	{
		float AverageWeight = TotalWeight / Times.Num();
		float AverageTime = TotalTime / Times.Num();

		CalibrationAsset->BaselineAnimation = AnimationSequence;
		CalibrationAsset->WeightSensitivity = AverageWeight * CalibrationHeadroom;
		CalibrationAsset->TimeSensitivity = AverageTime * CalibrationHeadroom;

		CalibrationAsset->MarkPackageDirty();

		if (bDebugNormalize)
		{
			FName DNEffortSpaceCurveName = TEXT("DebugNormalizedLabanEffortSpace");
			FName DNEffortWeightCurveName = TEXT("DebugNormalizedLabanEffortWeight");
			FName DNEffortTimeCurveName = TEXT("DebugNormalizedLabanEffortTime");

			UAnimationBlueprintLibrary::AddCurve(AnimationSequence, DNEffortSpaceCurveName, ERawCurveTrackTypes::RCT_Float);
			UAnimationBlueprintLibrary::AddCurve(AnimationSequence, DNEffortWeightCurveName, ERawCurveTrackTypes::RCT_Float);
			UAnimationBlueprintLibrary::AddCurve(AnimationSequence, DNEffortTimeCurveName, ERawCurveTrackTypes::RCT_Float);

			for (int i = 0; i < Times.Num(); ++i)
			{
				UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, DNEffortSpaceCurveName, Times[i], UUpStageMathFunctions::InverseRatio(EffortSpace[i]));
				UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, DNEffortWeightCurveName, Times[i], UUpStageMathFunctions::Normalize(EffortWeight[i], CalibrationAsset->WeightSensitivity));
				UAnimationBlueprintLibrary::AddFloatCurveKey(AnimationSequence, DNEffortTimeCurveName, Times[i], UUpStageMathFunctions::Normalize(EffortTime[i], CalibrationAsset->TimeSensitivity));
			}
		}
	}
}

void UUpStageLabanMovementModifier::OnRevert_Implementation(UAnimSequence* AnimationSequence)
{
	if (!AnimationSequence) return;

	FName EffortSpaceCurveName = TEXT("LabanEffortSpace");
	FName EffortWeightCurveName = TEXT("LabanEffortWeight");
	FName EffortTimeCurveName = TEXT("LabanEffortTime");
	
	if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, EffortSpaceCurveName, ERawCurveTrackTypes::RCT_Float))
		UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, EffortSpaceCurveName);

	if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, EffortWeightCurveName, ERawCurveTrackTypes::RCT_Float))
		UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, EffortWeightCurveName);

	if (UAnimationBlueprintLibrary::DoesCurveExist(AnimationSequence, EffortTimeCurveName, ERawCurveTrackTypes::RCT_Float))
		UAnimationBlueprintLibrary::RemoveCurve(AnimationSequence, EffortTimeCurveName);

	if (bCalibrate && CalibrationAsset)
	{
		CalibrationAsset->BaselineAnimation = nullptr;
		CalibrationAsset->WeightSensitivity = 0.f;
		CalibrationAsset->TimeSensitivity = 0.f;
		CalibrationAsset->MarkPackageDirty();
	}
}

FTransform UUpStageLabanMovementModifier::GetComponentSpaceTransform(UAnimSequence* AnimationSequence, FUpStageJointData TargetJoint, float Time) const
{
	FTransform ResultTransform = FTransform::Identity;

	USkeleton* Skeleton = AnimationSequence->GetSkeleton();
	if (!Skeleton) return ResultTransform;

	if (TargetJoint.BonePathToRoot.Num() == 0)
	{
		TArray<FName> BonePathToRoot;
		UAnimationBlueprintLibrary::FindBonePathToRoot(AnimationSequence, TargetJoint.BoneName, BonePathToRoot);
		TargetJoint.BonePathToRoot = BonePathToRoot;
	}

	for (int32 i = TargetJoint.BonePathToRoot.Num() - 1; i >= 0; --i)
	{
		FTransform LocalTransform;
		UAnimationBlueprintLibrary::GetBonePoseForTime(AnimationSequence, TargetJoint.BonePathToRoot[i], Time, false, LocalTransform);

		ResultTransform = LocalTransform * ResultTransform;
	}

	return ResultTransform;
}

FUpStageJointDynamics UUpStageLabanMovementModifier::CalculateJointDynamics(UAnimSequence* AnimationSequence, FUpStageJointData TargetJoint, TArray<float> Times) const
{
	FUpStageJointDynamics Dynamics;
	Dynamics.Positions.Reserve(Times.Num());
	Dynamics.Velocities.Init(FVector::ZeroVector, Times.Num());
	Dynamics.Accelerations.Init(FVector::ZeroVector, Times.Num());
	Dynamics.Jerks.Init(FVector::ZeroVector, Times.Num());

	for (int32 j = 0; j < Times.Num(); ++j)
	{
		// TODO: Require access to every time frame. Potential problem.
		FTransform JointTransform = GetComponentSpaceTransform(AnimationSequence, TargetJoint, Times[j]);
		Dynamics.Positions.Add(JointTransform.GetLocation());

		if (j > 0)
		{
			float DeltaTime = Times[j] - Times[j - 1];
			
			FVector DeltaPos = Dynamics.Positions[j] - Dynamics.Positions[j - 1];
			if (DeltaTime > KINDA_SMALL_NUMBER) Dynamics.Velocities[j] = DeltaPos / DeltaTime;
			
			if (j > 1)
			{
				FVector DeltaVel = Dynamics.Velocities[j] - Dynamics.Velocities[j - 1];
				if (DeltaTime > KINDA_SMALL_NUMBER) Dynamics.Accelerations[j] = DeltaVel / DeltaTime;
				
				if (j > 2)
				{
					FVector DeltaAcc = Dynamics.Accelerations[j] - Dynamics.Accelerations[j - 1];
					if (DeltaTime > KINDA_SMALL_NUMBER) Dynamics.Jerks[j] = DeltaAcc / DeltaTime;
				}
			}
		}
	}

	return Dynamics;
}

// Forward Window
//float UUpStageLabanMovementModifier::CalculateEffortSpace(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const
//{
//	int32 T = FrameIndex + TemporalWindowSize;
//	if (T >= NumFrames)
//	{
//		return 0.f;
//	}
//
//	int32 t0 = FrameIndex + ShortTimeWindowSize;
//	float EffortSpaceNumerator = 0.f;
//	float EffortSpaceDenominator = (JointDynamics.Positions[T] - JointDynamics.Positions[FrameIndex]).Size();
//
//	for (int32 t = t0; t <= T; ++t)
//	{
//		EffortSpaceNumerator += (JointDynamics.Positions[t] - JointDynamics.Positions[t - ShortTimeWindowSize]).Size();
//	}
//
//	return EffortSpaceDenominator > KINDA_SMALL_NUMBER ? EffortSpaceNumerator / EffortSpaceDenominator : 0.f;
//}

// Central Window
float UUpStageLabanMovementModifier::CalculateEffortSpace(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const
{
	int32 HalfWindow = TemporalWindowSize / 2;
	int32 StartIndex = FMath::Max(0, FrameIndex - HalfWindow);
	int32 EndIndex = FMath::Min(NumFrames - 1, FrameIndex + HalfWindow);

	if (EndIndex <= StartIndex) return 1.f; // No movement if we don't have a valid window

	float EffortSpaceNumerator = 0.f;
	float EffortSpaceDenominator = (JointDynamics.Positions[EndIndex] - JointDynamics.Positions[StartIndex]).Size();

	int32 t0 = StartIndex + ShortTimeWindowSize;
	for (int32 t = t0; t <= EndIndex; ++t)
	{
		// Safe check to prevent looking outside the window boundaries
		int32 PrevT = FMath::Max(StartIndex, t - ShortTimeWindowSize);
		EffortSpaceNumerator += (JointDynamics.Positions[t] - JointDynamics.Positions[PrevT]).Size();
	}

	return EffortSpaceDenominator > KINDA_SMALL_NUMBER ? EffortSpaceNumerator / EffortSpaceDenominator : 1.0f;
}

float UUpStageLabanMovementModifier::CalculateEffortWeight(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const
{
	if (AnalysisType == EUpStageLabanMovementAnalysisType::Kinematic)
	{
		float Speed = JointDynamics.Velocities[FrameIndex].Size();
		return 0.5f * (Speed * Speed);
	}

	float VerticalAcceleration = JointDynamics.Accelerations[FrameIndex].Z;
	float Force = JointDynamics.Accelerations[FrameIndex].Size(); // Assuming unit mass for simplicity
	float GravityModifier = (VerticalAcceleration < 0.f) ? 1.5f : 0.5f; // Upward movements are weighted more than downward
	return Force * GravityModifier;
}

// Forward Window
//float UUpStageLabanMovementModifier::CalculateEffortTime(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const
//{
//	int32 T = FrameIndex + TemporalWindowSize;
//	if (T >= NumFrames)
//	{
//		return 0.f;
//	}
//
//	float EffortTimeAccum = 0.f;
//	for (int32 t = FrameIndex; t <= T; ++t)
//	{
//		switch (AnalysisType)
//		{
//		case EUpStageLabanMovementAnalysisType::Kinematic:
//			EffortTimeAccum += JointDynamics.Accelerations[t].Size();
//			break;
//		case EUpStageLabanMovementAnalysisType::Newtonian:
//			EffortTimeAccum += JointDynamics.Jerks[t].Size();
//			break;
//		default:
//			break;
//		}
//	}
//
//	return EffortTimeAccum / TemporalWindowSize;
//}

// Central Window
float UUpStageLabanMovementModifier::CalculateEffortTime(FUpStageJointDynamics JointDynamics, int32 FrameIndex, int32 NumFrames) const
{
	int32 HalfWindow = TemporalWindowSize / 2;
	int32 StartFrame = FMath::Max(0, FrameIndex - HalfWindow);
	int32 EndFrame = FMath::Min(NumFrames - 1, FrameIndex + HalfWindow);

	int32 ActualWindowSize = (EndFrame - StartFrame) + 1;
	if (ActualWindowSize <= 0) return 0.f; // Safety check

	float EffortTimeAccum = 0.f;

	for (int32 t = StartFrame; t <= EndFrame; ++t)
	{
		switch (AnalysisType)
		{
		case EUpStageLabanMovementAnalysisType::Kinematic:
			EffortTimeAccum += JointDynamics.Accelerations[t].Size();
			break;
		case EUpStageLabanMovementAnalysisType::Newtonian:
			EffortTimeAccum += JointDynamics.Jerks[t].Size();
			break;
		default:
			break;
		}
	}

	return EffortTimeAccum / ActualWindowSize;
}