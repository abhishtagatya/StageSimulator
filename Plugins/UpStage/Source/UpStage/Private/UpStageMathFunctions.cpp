// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageMathFunctions.h"

float UUpStageMathFunctions::Normalize(const float X, const float K)
{
	return X / (X + K);
}

float UUpStageMathFunctions::InverseRatio(const float X)
{
	if (X <= 0.f) return 0.f;
	return 1.0f - (1.0f / X);
}

EUpStageLabanEffortAction UUpStageMathFunctions::SelectEffortAction(FUpStageFrameMovementAnalysis FrameMovementAnalysis, FVector2D Threshold)
{
    bool bIsIndirect = false;
    bool bIsStrong = false;
    bool bIsSudden = false;

    switch (FrameMovementAnalysis.EffortAction)
    {
        case EUpStageLabanEffortAction::Punch:  bIsStrong = true; bIsSudden = true; break;
        case EUpStageLabanEffortAction::Slash:  bIsIndirect = true; bIsStrong = true; bIsSudden = true; break;
        case EUpStageLabanEffortAction::Dab:    bIsSudden = true; break;
        case EUpStageLabanEffortAction::Flick:  bIsIndirect = true; bIsSudden = true; break;
        case EUpStageLabanEffortAction::Press:  bIsStrong = true; break;
        case EUpStageLabanEffortAction::Wring:  bIsIndirect = true; bIsStrong = true; break;
        case EUpStageLabanEffortAction::Float:  bIsIndirect = true; break;
        case EUpStageLabanEffortAction::Glide:  break;
        default: break; // Neutral
    }

    float DeactivateThreshold = Threshold.X;
    float ActivateThreshold = Threshold.Y;

    if (!bIsIndirect && FrameMovementAnalysis.EffortSpace > ActivateThreshold) bIsIndirect = true;
    else if (bIsIndirect && FrameMovementAnalysis.EffortSpace < DeactivateThreshold) bIsIndirect = false;

    if (!bIsStrong && FrameMovementAnalysis.EffortWeight > ActivateThreshold) bIsStrong = true;
    else if (bIsStrong && FrameMovementAnalysis.EffortWeight < DeactivateThreshold) bIsStrong = false;

    if (!bIsSudden && FrameMovementAnalysis.EffortTime > ActivateThreshold) bIsSudden = true;
    else if (bIsSudden && FrameMovementAnalysis.EffortTime < DeactivateThreshold) bIsSudden = false;

    if (!bIsIndirect && bIsStrong && bIsSudden)        return EUpStageLabanEffortAction::Punch;
    if (bIsIndirect && bIsStrong && bIsSudden)         return EUpStageLabanEffortAction::Slash;
    if (!bIsIndirect && !bIsStrong && bIsSudden)       return EUpStageLabanEffortAction::Dab;
    if (bIsIndirect && !bIsStrong && bIsSudden)        return EUpStageLabanEffortAction::Flick;
    if (!bIsIndirect && bIsStrong && !bIsSudden)       return EUpStageLabanEffortAction::Press;
    if (bIsIndirect && bIsStrong && !bIsSudden)        return EUpStageLabanEffortAction::Wring;
    if (!bIsIndirect && !bIsStrong && !bIsSudden)      return EUpStageLabanEffortAction::Glide;
    if (bIsIndirect && !bIsStrong && !bIsSudden)       return EUpStageLabanEffortAction::Float;

    return FrameMovementAnalysis.EffortAction;
}