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

            if (FrameScore > Parameter.MinPerformerScore)
            {
                FUpStageKeyMomentEvaluation& ExistingEvaluation = KeyMomentEvaluations.FindOrAdd(FrameIndex);
                ExistingEvaluation.FrameIndex = FrameIndex;
                ExistingEvaluation.KeyMomentScore += FrameScore;
                ExistingEvaluation.Actors.Add(Actor);
                ExistingEvaluation.FocalStructures.Add(MovementAnalysis.FrameFocalStructures[FrameIndex]);
			}
        }
    }

	// Convert the map to an array for easier use in Blueprints
	KeyMomentEvaluations.GenerateValueArray(KeyMomentsArray);
	return KeyMomentsArray;
}

TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::ExtractKeyMomentsFromSelectedFrames(const TMap<AActor*, FUpStageSequencerMovementAnalysis>& PerformerFrameActions, const FUpStageSequencerKeyMomentParameter& Parameter, const TArray<int32>& Frames)
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

            if (Frames.Contains(FrameIndex))
            {
                FUpStageKeyMomentEvaluation& ExistingEvaluation = KeyMomentEvaluations.FindOrAdd(FrameIndex);
                ExistingEvaluation.FrameIndex = FrameIndex;
                ExistingEvaluation.KeyMomentScore += FrameScore;
                ExistingEvaluation.Actors.Add(Actor);
                ExistingEvaluation.FocalStructures.Add(MovementAnalysis.FrameFocalStructures[FrameIndex]);
            }
        }
    }

    // Convert the map to an array for easier use in Blueprints
    KeyMomentEvaluations.GenerateValueArray(KeyMomentsArray);
    return KeyMomentsArray;
}

TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::SelectKeyMoments(const TArray<FUpStageKeyMomentEvaluation>& KeyMomentEvaluations, const FUpStageSequencerKeyMomentParameter& Parameter)
{
    TArray<FUpStageKeyMomentEvaluation> SortedEvaluations = KeyMomentEvaluations;
    SortedEvaluations.Sort([](const FUpStageKeyMomentEvaluation& A, const FUpStageKeyMomentEvaluation& B) {
        return A.KeyMomentScore > B.KeyMomentScore;
    });

    if (Parameter.bUseNonMaximumSupression) SortedEvaluations = NonMaximumSuppression(SortedEvaluations, Parameter.MinFrameDistance);
    if (SortedEvaluations.Num() > Parameter.TopN) SortedEvaluations.SetNum(Parameter.TopN);

    return SortedEvaluations;
}

TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::NonMaximumSuppression(const TArray<FUpStageKeyMomentEvaluation>& SortedKeyMoments, int32 MinFrameDistance)
{
    TArray<FUpStageKeyMomentEvaluation> FilteredMoments;
    for (const FUpStageKeyMomentEvaluation& CurrentEval : SortedKeyMoments)
    {
        bool bIsSuppressed = false;

        for (const FUpStageKeyMomentEvaluation& KeptEval : FilteredMoments)
        {
            if (FMath::Abs(CurrentEval.FrameIndex - KeptEval.FrameIndex) < MinFrameDistance)
            {
                bIsSuppressed = true;
                break;
            }
        }

        if (!bIsSuppressed)
        {
            FilteredMoments.Add(CurrentEval);
        }
    }

    return FilteredMoments;
}

ACineCameraActor* UUpStageAnalysisFunctions::SelectBestCameraForKeyMoment(const TArray<ACineCameraActor*>& CameraArray, const FUpStageKeyMomentEvaluation& KeyMoment, const FUpStageSequencerCameraCutsParameter& Parameter)
{
    if (KeyMoment.FocalStructures.IsEmpty() || CameraArray.IsEmpty()) return nullptr;

	TMap<ACineCameraActor*, float> CameraScores;

	FVector PerformerSumLocation = FVector::ZeroVector;
	FVector PerformerSumForward = FVector::ZeroVector;

    for (const FUpStagePerformerFocalStructure& FocalStruct : KeyMoment.FocalStructures)
    {
        PerformerSumLocation += FocalStruct.MainFocalTransform.GetLocation();
        PerformerSumForward += FocalStruct.MainFocalDirection;
	}

	FVector AlignmentCentroid = PerformerSumLocation / KeyMoment.FocalStructures.Num();
	FVector AlignmentForward = PerformerSumForward.GetSafeNormal();

    for (ACineCameraActor* Camera : CameraArray)
    {
        float AlignmentScore = CalculateAlignmentScore(Camera, AlignmentCentroid, AlignmentForward, Parameter.CameraSelectionParameter);
		float VisibilityScore = CalculateVisibilityScore(Camera, KeyMoment.FocalStructures, Parameter.CameraSelectionParameter);
        float CompositionScore = 0.f; // Placeholder for actual composition calculation
        float TotalScore = AlignmentScore + VisibilityScore + CompositionScore;
        CameraScores.Add(Camera, TotalScore);
	}

	ACineCameraActor* BestCamera = nullptr;
    for (const TPair<ACineCameraActor*, float>& Pair : CameraScores)
    {
        if (!BestCamera || Pair.Value > CameraScores[BestCamera])
        {
            BestCamera = Pair.Key;
        }
	}

    return BestCamera;
}

float UUpStageAnalysisFunctions::CalculateAlignmentScore(ACineCameraActor* Camera, FVector Centroid, FVector Forward, const FUpStageCameraSelectionParameter& Parameter)
{
    if (!Camera) return 0.f;
    FVector CameraLocation = Camera->GetActorLocation();
    FVector CameraForward = Camera->GetActorForwardVector();
    float CurrentDot = FVector::DotProduct((CameraLocation - Centroid).GetSafeNormal(), Forward);
    return ((CurrentDot + 1.0f) / 2.0f) * Parameter.AlignmentScore;
}

float UUpStageAnalysisFunctions::CalculateVisibilityScore(ACineCameraActor* Camera, const TArray<FUpStagePerformerFocalStructure>& FocalStructures, const FUpStageCameraSelectionParameter& Parameter)
{
	if (!Camera || FocalStructures.Num() == 0) return 0.f;

	UWorld* World = Camera->GetWorld();
	if (!World) return 0.f;

    float TotalVisibilityScore = 0.0f;

    FVector CameraLocation = Camera->GetActorLocation();
    FCollisionQueryParams TraceParams(FName(TEXT("CameraVisibilityTrace")), Parameter.bUseComplexTrace);
    TraceParams.AddIgnoredActor(Camera);

    for (const FUpStagePerformerFocalStructure& FocalStruct : FocalStructures)
    {
        FVector TargetLocation = FocalStruct.MainFocalTransform.GetLocation();

        bool bIsObstructed = World->LineTraceTestByChannel(
            CameraLocation,
            TargetLocation,
            ECC_Visibility,
            TraceParams
        );

		TotalVisibilityScore += Parameter.VisibilityScore * !(bIsObstructed);

        for (const FUpStagePerformerFocalMember& Member : FocalStruct.FocalMembers)
        {
            FVector MemberLocation = Member.FocalMemberTransform.GetLocation();
            bool bIsMemberObstructed = World->LineTraceTestByChannel(
                CameraLocation,
                MemberLocation,
                ECC_Visibility,
                TraceParams
            );

            TotalVisibilityScore += Parameter.VisibilityScore * Member.ImportanceScore * !(bIsMemberObstructed);
        }
    }

    return TotalVisibilityScore;
}

float UUpStageAnalysisFunctions::CalculateEffortIntensity(const FUpStageFrameMovementAnalysis& FrameMovementAnalysis)
{
    return FMath::Abs(FrameMovementAnalysis.EffortSpace - 0.5f) * 2.f + FMath::Abs(FrameMovementAnalysis.EffortWeight - 0.5f) * 2.f + FMath::Abs(FrameMovementAnalysis.EffortTime - 0.5f) * 2.f;
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
    if (CurrentValue < PrevExtremity.MinValue)
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
