// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageAnalysisFunctions.h"


TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::ExtractKeyMoments(const TMap<AActor*, FUpStageSequencerMovementAnalysis>& PerformerFrameActions, const FUpStageSequencerKeyMomentParameter& Parameter)
{
	TMap<int32, FUpStageKeyMomentEvaluation> KeyMomentEvaluations;
	TArray<FUpStageKeyMomentEvaluation> KeyMomentsArray;

	FUpStageActionTransitionAnalysisParameter ActionTransitionParameter;

	// Preload previous frame effort actions for transition analysis
	TMap<AActor*, EUpStageLabanEffortAction> PreviousFrameEffortActions;
    for (const TPair<AActor*, FUpStageSequencerMovementAnalysis>& Pair : PerformerFrameActions)
    {
		AActor* Actor = Pair.Key;
        if (!Actor) continue;
		PreviousFrameEffortActions.Add(Actor, ActionTransitionParameter.BaseEffortAction);
	}

    for (const TPair<AActor*, FUpStageSequencerMovementAnalysis>& Pair : PerformerFrameActions)
    {
        AActor* Actor = Pair.Key;
        const FUpStageSequencerMovementAnalysis& MovementAnalysis = Pair.Value;

        if (!Actor) continue;

		int32 TotalFrames = MovementAnalysis.FrameMovementAnalyses.Num();
        for (int FrameIndex = 0; FrameIndex < TotalFrames; ++FrameIndex)
        {
            if (FrameIndex <= Parameter.FramePadding || FrameIndex >= TotalFrames - Parameter.FramePadding) continue;

            int FrameScore = 0;

            if (Parameter.bAnalyzeActionTransition)
            {
				EUpStageLabanEffortAction CurrentEffortAction = TransitionEffortAction(
                    PreviousFrameEffortActions[Actor], 
                    MovementAnalysis.FrameMovementAnalyses[FrameIndex], 
                    ActionTransitionParameter.ActionTransitionThreshold);

                if (CurrentEffortAction != PreviousFrameEffortActions[Actor])
                {
                    FrameScore += CalculateEffortTransitionScore(
                        PreviousFrameEffortActions[Actor], 
                        CurrentEffortAction, 
                        ActionTransitionParameter);
				}

				PreviousFrameEffortActions[Actor] = CurrentEffortAction;
			}

            if (Parameter.bAnalyzeExtremities)
            {
                // Analyze effort actions
                // This is a placeholder for the actual analysis logic, which would likely involve using the SelectEffortAction function to determine the effort action for the current frame and comparing it with previous frames to detect changes.
            }

            if (FrameScore > 0)
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

EUpStageLabanEffortAction UUpStageAnalysisFunctions::TransitionEffortAction(EUpStageLabanEffortAction EffortAction, FUpStageFrameMovementAnalysis FrameMovementAnalysis, FVector2D Threshold)
{
    bool bIsIndirect = false;
    bool bIsStrong = false;
    bool bIsSudden = false;

    switch (EffortAction)
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

    return EffortAction;
}

int UUpStageAnalysisFunctions::CalculateEffortTransitionScore(EUpStageLabanEffortAction PreviousEffortAction, EUpStageLabanEffortAction CurrentEffortAction, const FUpStageActionTransitionAnalysisParameter& Parameter)
{
    if (PreviousEffortAction == CurrentEffortAction) return 0;

    bool bPrevIndirect = false;
    bool bPrevStrong = false;
    bool bPrevSudden = false;

    switch (PreviousEffortAction)
    {
	case EUpStageLabanEffortAction::Punch:  bPrevStrong = true; bPrevSudden = true; break;
	case EUpStageLabanEffortAction::Slash:  bPrevIndirect = true; bPrevStrong = true; bPrevSudden = true; break;
	case EUpStageLabanEffortAction::Dab:    bPrevSudden = true; break;
	case EUpStageLabanEffortAction::Flick:  bPrevIndirect = true; bPrevSudden = true; break;
	case EUpStageLabanEffortAction::Press:  bPrevStrong = true; break;
	case EUpStageLabanEffortAction::Wring:  bPrevIndirect = true; bPrevStrong = true; break;
	case EUpStageLabanEffortAction::Float:  bPrevIndirect = true; break;
	case EUpStageLabanEffortAction::Glide:  break;
    default: break; // Neutral
    }

	bool bCurrIndirect = false;
	bool bCurrStrong = false;
	bool bCurrSudden = false;

    switch (CurrentEffortAction)
	{
	case EUpStageLabanEffortAction::Punch:  bCurrStrong = true; bCurrSudden = true; break;
	case EUpStageLabanEffortAction::Slash:  bCurrIndirect = true; bCurrStrong = true; bCurrSudden = true; break;
	case EUpStageLabanEffortAction::Dab:    bCurrSudden = true; break;
	case EUpStageLabanEffortAction::Flick:  bCurrIndirect = true; bCurrSudden = true; break;
	case EUpStageLabanEffortAction::Press:  bCurrStrong = true; break;
	case EUpStageLabanEffortAction::Wring:  bCurrIndirect = true; bCurrStrong = true; break;
	case EUpStageLabanEffortAction::Float:  bCurrIndirect = true; break;
    case EUpStageLabanEffortAction::Glide:  break;
    default: break; // Neutral
    }
    
    int Score = 0;
    if (bPrevIndirect != bCurrIndirect) Score += Parameter.EffortTransitionScore;
    if (bPrevStrong != bCurrStrong) Score += Parameter.EffortTransitionScore;
    if (bPrevSudden != bCurrSudden) Score += Parameter.EffortTransitionScore;

	return Score;
}
