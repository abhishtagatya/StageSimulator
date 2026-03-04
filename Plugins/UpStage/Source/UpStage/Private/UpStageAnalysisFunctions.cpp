// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageAnalysisFunctions.h"


TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::ExtractKeyMoments(const TMap<AActor*, FUpStageSequencerMovementAnalysis>& PerformerFrameActions, const FUpStageSequencerKeyMomentParameter& Parameter)
{
	TMap<int32, FUpStageKeyMomentEvaluation> KeyMomentEvaluations;
	TArray<FUpStageKeyMomentEvaluation> KeyMomentsArray;

    for (const TPair<AActor*, FUpStageSequencerMovementAnalysis>& Pair : PerformerFrameActions)
    {
        AActor* Actor = Pair.Key;
        const FUpStageSequencerMovementAnalysis& MovementAnalysis = Pair.Value;

        if (!Actor) continue;

        EUpStageLabanEffortAction PrevEffortAction = Parameter.ActionTransitionAnalysisParameter.BaseEffortAction;
        FUpStageLabanEffortExtremities PrevExtremity;
        PrevExtremity.SpaceExtremity.MinValue = PrevExtremity.SpaceExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortSpace;
        PrevExtremity.WeightExtremity.MinValue = PrevExtremity.WeightExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortWeight;
        PrevExtremity.TimeExtremity.MinValue = PrevExtremity.TimeExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortTime;

        int32 TotalFrames = MovementAnalysis.FrameMovementAnalyses.Num();
        for (int FrameIndex = 0; FrameIndex < TotalFrames; ++FrameIndex)
        {
            if (FrameIndex <= Parameter.FramePadding || FrameIndex >= TotalFrames - Parameter.FramePadding) continue;

            float FrameScore = CalculateEffortIntensity(MovementAnalysis.FrameMovementAnalyses[FrameIndex]);

            if (Parameter.bAnalyzeActionTransition)
            {
				EUpStageLabanEffortAction CurrentEffortAction = TransitionEffortAction(
                    PrevEffortAction,
                    MovementAnalysis.FrameMovementAnalyses[FrameIndex], 
                    Parameter.ActionTransitionAnalysisParameter.ActionTransitionThreshold);

                if (CurrentEffortAction != PrevEffortAction)
                {
                    FrameScore += CalculateEffortTransitionScore(
                        PrevEffortAction,
                        CurrentEffortAction, 
                        Parameter.ActionTransitionAnalysisParameter);
				}

                PrevEffortAction = CurrentEffortAction;
			}

            if (Parameter.bAnalyzeExtremities)
            {
                // Analyze effort actions
                // This is a placeholder for the actual analysis logic, which would likely involve using the SelectEffortAction function to determine the effort action for the current frame and comparing it with previous frames to detect changes.

                FrameScore += CalculateEffortExtremityScore(
                    MovementAnalysis.FrameMovementAnalyses[FrameIndex], 
                    PrevExtremity,
                    Parameter.ExtremityAnalysisParameter);
            }

            if (FrameScore > 10.f)
            {
                FUpStageKeyMomentEvaluation& ExistingEvaluation = KeyMomentEvaluations.FindOrAdd(FrameIndex);
                ExistingEvaluation.FrameIndex = FrameIndex;
                ExistingEvaluation.KeyMomentScore += FrameScore;
                ExistingEvaluation.Actors.Add(Actor);
			}
        }
    }

	// Convert the map to an array for easier use in Blueprints
	KeyMomentEvaluations.GenerateValueArray(KeyMomentsArray);
	return KeyMomentsArray;
}

float UUpStageAnalysisFunctions::CalculateEffortIntensity(const FUpStageFrameMovementAnalysis& FrameMovementAnalysis)
{
	return FMath::Abs(FrameMovementAnalysis.EffortSpace - 0.5f) * 2.f + FMath::Abs(FrameMovementAnalysis.EffortWeight - 0.5f) * 2.f + FMath::Abs(FrameMovementAnalysis.EffortTime - 0.5f) * 2.f;
}

TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::SelectKeyMoments(const TArray<FUpStageKeyMomentEvaluation>& KeyMomentEvaluations, int32 TopN)
{
    TArray<FUpStageKeyMomentEvaluation> SortedEvaluations = KeyMomentEvaluations;
    SortedEvaluations.Sort([](const FUpStageKeyMomentEvaluation& A, const FUpStageKeyMomentEvaluation& B) {
        return A.KeyMomentScore > B.KeyMomentScore;
    });

    TArray<FUpStageKeyMomentEvaluation> SelectedKeyMoments;
    for (int32 i = 0; i < FMath::Min(TopN, SortedEvaluations.Num()); ++i)
    {
        SelectedKeyMoments.Add(SortedEvaluations[i]);
    }
    return SelectedKeyMoments;
}

FUpStageLabanEffortActionDimension UUpStageAnalysisFunctions::GetEffortDimensions(EUpStageLabanEffortAction Action)
{
    FUpStageLabanEffortActionDimension Dims = { false, false, false }; // Default to Glide
    switch (Action)
    {
    case EUpStageLabanEffortAction::Punch:  Dims.bIsStrong = true; Dims.bIsSudden = true; break;
    case EUpStageLabanEffortAction::Slash:  Dims.bIsIndirect = true; Dims.bIsStrong = true; Dims.bIsSudden = true; break;
    case EUpStageLabanEffortAction::Dab:    Dims.bIsSudden = true; break;
    case EUpStageLabanEffortAction::Flick:  Dims.bIsIndirect = true; Dims.bIsSudden = true; break;
    case EUpStageLabanEffortAction::Press:  Dims.bIsStrong = true; break;
    case EUpStageLabanEffortAction::Wring:  Dims.bIsIndirect = true; Dims.bIsStrong = true; break;
    case EUpStageLabanEffortAction::Float:  Dims.bIsIndirect = true; break;
    case EUpStageLabanEffortAction::Glide:  break;
    default: break;
    }
    return Dims;
}

EUpStageLabanEffortAction UUpStageAnalysisFunctions::GetEffortAction(FUpStageLabanEffortActionDimension Dimension)
{
    if (!Dimension.bIsIndirect && Dimension.bIsStrong && Dimension.bIsSudden)        return EUpStageLabanEffortAction::Punch;
    if (Dimension.bIsIndirect && Dimension.bIsStrong && Dimension.bIsSudden)         return EUpStageLabanEffortAction::Slash;
    if (!Dimension.bIsIndirect && !Dimension.bIsStrong && Dimension.bIsSudden)       return EUpStageLabanEffortAction::Dab;
    if (Dimension.bIsIndirect && !Dimension.bIsStrong && Dimension.bIsSudden)        return EUpStageLabanEffortAction::Flick;
    if (!Dimension.bIsIndirect && Dimension.bIsStrong && !Dimension.bIsSudden)       return EUpStageLabanEffortAction::Press;
    if (Dimension.bIsIndirect && Dimension.bIsStrong && !Dimension.bIsSudden)        return EUpStageLabanEffortAction::Wring;
    if (!Dimension.bIsIndirect && !Dimension.bIsStrong && !Dimension.bIsSudden)      return EUpStageLabanEffortAction::Glide;
    if (Dimension.bIsIndirect && !Dimension.bIsStrong && !Dimension.bIsSudden)       return EUpStageLabanEffortAction::Float;
    return EUpStageLabanEffortAction::Glide;
}

EUpStageLabanEffortAction UUpStageAnalysisFunctions::TransitionEffortAction(EUpStageLabanEffortAction EffortAction, FUpStageFrameMovementAnalysis FrameMovementAnalysis, FVector2D Threshold)
{
    FUpStageLabanEffortActionDimension Prev = GetEffortDimensions(EffortAction);
    
    float DeactivateThreshold = Threshold.X;
    float ActivateThreshold = Threshold.Y;

    if (!Prev.bIsIndirect && FrameMovementAnalysis.EffortSpace > ActivateThreshold) Prev.bIsIndirect = true;
    else if (Prev.bIsIndirect && FrameMovementAnalysis.EffortSpace < DeactivateThreshold) Prev.bIsIndirect = false;

    if (!Prev.bIsStrong && FrameMovementAnalysis.EffortWeight > ActivateThreshold) Prev.bIsStrong = true;
    else if (Prev.bIsStrong && FrameMovementAnalysis.EffortWeight < DeactivateThreshold) Prev.bIsStrong = false;

    if (!Prev.bIsSudden && FrameMovementAnalysis.EffortTime > ActivateThreshold) Prev.bIsSudden = true;
    else if (Prev.bIsSudden && FrameMovementAnalysis.EffortTime < DeactivateThreshold) Prev.bIsSudden = false;

    return GetEffortAction(Prev);
}

float UUpStageAnalysisFunctions::CalculateEffortTransitionScore(EUpStageLabanEffortAction PreviousEffortAction, EUpStageLabanEffortAction CurrentEffortAction, const FUpStageActionTransitionAnalysisParameter& Parameter)
{
    if (PreviousEffortAction == CurrentEffortAction) return 0.f;

    FUpStageLabanEffortActionDimension Prev = GetEffortDimensions(PreviousEffortAction);
    FUpStageLabanEffortActionDimension Curr = GetEffortDimensions(CurrentEffortAction);

    int Score = 0.f;
    if (Prev.bIsIndirect != Curr.bIsIndirect) Score += Parameter.EffortTransitionScore;
    if (Prev.bIsStrong != Curr.bIsStrong) Score += Parameter.EffortTransitionScore;
    if (Prev.bIsSudden != Curr.bIsSudden) Score += Parameter.EffortTransitionScore;

    return Score;
}

float UUpStageAnalysisFunctions::CalculateEffortExtremityScore(const FUpStageFrameMovementAnalysis& FrameMovementAnalysis, FUpStageLabanEffortExtremities& EffortExtremtities, const FUpStageExtremityAnalysisParameter& Parameter)
{
    float Score = 0.f;
	Score += ExtremityDetection(FrameMovementAnalysis.EffortSpace, EffortExtremtities.SpaceExtremity, Parameter.Tolerance) ? Parameter.ExtremityScore : 0.f;
	Score += ExtremityDetection(FrameMovementAnalysis.EffortWeight, EffortExtremtities.WeightExtremity, Parameter.Tolerance) ? Parameter.ExtremityScore : 0.f;
	Score += ExtremityDetection(FrameMovementAnalysis.EffortTime, EffortExtremtities.TimeExtremity, Parameter.Tolerance) ? Parameter.ExtremityScore : 0.f;
    return Score;
}

bool UUpStageAnalysisFunctions::ExtremityDetection(float CurrentValue, FUpStageExtremityStructure& PrevExtremity, float Tolerance)
{
    if (CurrentValue > PrevExtremity.MaxValue)
    {
        PrevExtremity.MaxValue = CurrentValue;
    }
    else
    {
        PrevExtremity.MinValue = CurrentValue;
    }

    if (PrevExtremity.bLookForPeak)
    {
        if (CurrentValue < PrevExtremity.MaxValue - Tolerance)
        {
            // This is Peak
            PrevExtremity.MinValue = CurrentValue;
            PrevExtremity.bLookForPeak = false;
            return true;
        }
    }
    else
    {
        if (CurrentValue > PrevExtremity.MinValue + Tolerance)
        {
            // This is Valley
            PrevExtremity.MaxValue = CurrentValue;
            PrevExtremity.bLookForPeak = true;
            return true;
        }
    }

    return false;
}
