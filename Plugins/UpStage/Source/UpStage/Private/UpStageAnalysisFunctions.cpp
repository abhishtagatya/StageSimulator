// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageAnalysisFunctions.h"


TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::ExtractKeyMoments(
	const TMap<AActor*, FUpStageSequencerMovementAnalysis>& PerformerFrameActions, 
	const FUpStageSequencerKeyMomentParameter& Parameter,
	const TArray<int32>& ManualFrames,
	const FIntPoint& PlaybackBounds)
{
	TMap<int32, FUpStageKeyMomentEvaluation> KeyMomentEvaluations;
	TArray<FUpStageKeyMomentEvaluation> KeyMomentsArray;

	TSet<int32> ManualFramesSet(ManualFrames);

	for (const TPair<AActor*, FUpStageSequencerMovementAnalysis>& Pair : PerformerFrameActions)
	{
		AActor* Actor = Pair.Key;
		const FUpStageSequencerMovementAnalysis& MovementAnalysis = Pair.Value;

		if (!Actor) continue;
		if ((MovementAnalysis.FrameMovementAnalyses.IsEmpty())) continue;

		EUpStageLabanEffortAction PrevEffortAction = Parameter.ActionTransitionAnalysisParameter.BaseEffortAction;
		FUpStageLabanEffortExtremities PrevExtremity;
		PrevExtremity.SpaceExtremity.MinValue = PrevExtremity.SpaceExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortSpace;
		PrevExtremity.WeightExtremity.MinValue = PrevExtremity.WeightExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortWeight;
		PrevExtremity.TimeExtremity.MinValue = PrevExtremity.TimeExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortTime;

		for (int FrameIndex = PlaybackBounds.X; FrameIndex <= PlaybackBounds.Y; ++FrameIndex)
		{
			if (FrameIndex <= PlaybackBounds.X + Parameter.FramePadding || FrameIndex >= PlaybackBounds.Y - Parameter.FramePadding) continue;

			int32 ArrayIndex = FrameIndex - PlaybackBounds.X;
			float FrameScore = CalculateEffortIntensity(MovementAnalysis.FrameMovementAnalyses[ArrayIndex]);
			bool bIsManual = ManualFramesSet.Contains(FrameIndex);

			if (Parameter.bAnalyzeActionTransition)
			{
				EUpStageLabanEffortAction CurrentEffortAction = TransitionEffortAction(
					PrevEffortAction,
					MovementAnalysis.FrameMovementAnalyses[ArrayIndex],
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
				FrameScore += CalculateEffortExtremityScore(
					MovementAnalysis.FrameMovementAnalyses[ArrayIndex],
					PrevExtremity,
					Parameter.ExtremityAnalysisParameter);
			}

			if (bIsManual || FrameScore > Parameter.MinPerformerScore)
			{
				FUpStageKeyMomentEvaluation& ExistingEvaluation = KeyMomentEvaluations.FindOrAdd(FrameIndex);
				ExistingEvaluation.FrameIndex = FrameIndex;
				ExistingEvaluation.bIsManualOverride = bIsManual;

				if (FrameScore > Parameter.MinPerformerScore)
				{
					ExistingEvaluation.KeyMomentScore += FrameScore;
					ExistingEvaluation.Actors.Add(Actor);
					ExistingEvaluation.FocalStructures.Add(MovementAnalysis.FrameFocalStructures[ArrayIndex]);
				}
			}
		}
	}	

	KeyMomentEvaluations.GenerateValueArray(KeyMomentsArray);
	return KeyMomentsArray;
}

TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::ExtractKeyMomentsFromSelectedFrames(
	const TMap<AActor*, FUpStageSequencerMovementAnalysis>& PerformerFrameActions, 
	const FUpStageSequencerKeyMomentParameter& Parameter, 
	const TArray<int32>& Frames,
	const FIntPoint& PlaybackBounds)
{
	TMap<int32, FUpStageKeyMomentEvaluation> KeyMomentEvaluations;
	TArray<FUpStageKeyMomentEvaluation> KeyMomentsArray;
	TSet<int32> FrameSet(Frames);

	for (int32 FrameIndex : Frames)
	{
		FUpStageKeyMomentEvaluation& Eval = KeyMomentEvaluations.FindOrAdd(FrameIndex);
		Eval.FrameIndex = FrameIndex;
		Eval.KeyMomentScore = 0.0f;
		Eval.bIsManualOverride = true;
	}

	for (const TPair<AActor*, FUpStageSequencerMovementAnalysis>& Pair : PerformerFrameActions)
	{
		AActor* Actor = Pair.Key;
		const FUpStageSequencerMovementAnalysis& MovementAnalysis = Pair.Value;

		if (!Actor) continue;
		if ((MovementAnalysis.FrameMovementAnalyses.IsEmpty())) continue;

		EUpStageLabanEffortAction PrevEffortAction = Parameter.ActionTransitionAnalysisParameter.BaseEffortAction;
		FUpStageLabanEffortExtremities PrevExtremity;
		PrevExtremity.SpaceExtremity.MinValue = PrevExtremity.SpaceExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortSpace;
		PrevExtremity.WeightExtremity.MinValue = PrevExtremity.WeightExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortWeight;
		PrevExtremity.TimeExtremity.MinValue = PrevExtremity.TimeExtremity.MaxValue = MovementAnalysis.FrameMovementAnalyses[0].EffortTime;

		for (int FrameIndex = PlaybackBounds.X; FrameIndex <= PlaybackBounds.Y; ++FrameIndex)
		{
			int32 ArrayIndex = FrameIndex - PlaybackBounds.X;
			float FrameScore = CalculateEffortIntensity(MovementAnalysis.FrameMovementAnalyses[ArrayIndex]);

			if (Parameter.bAnalyzeActionTransition)
			{
				EUpStageLabanEffortAction CurrentEffortAction = TransitionEffortAction(
					PrevEffortAction,
					MovementAnalysis.FrameMovementAnalyses[ArrayIndex],
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
				FrameScore += CalculateEffortExtremityScore(
					MovementAnalysis.FrameMovementAnalyses[ArrayIndex],
					PrevExtremity,
					Parameter.ExtremityAnalysisParameter);
			}

			if (FrameSet.Contains(FrameIndex))
			{
				FUpStageKeyMomentEvaluation& ExistingEvaluation = KeyMomentEvaluations.FindOrAdd(FrameIndex);
				ExistingEvaluation.FrameIndex = FrameIndex;

				if (FrameScore > Parameter.MinPerformerScore)
				{
					ExistingEvaluation.KeyMomentScore += FrameScore;
					ExistingEvaluation.Actors.Add(Actor);
					ExistingEvaluation.FocalStructures.Add(MovementAnalysis.FrameFocalStructures[ArrayIndex]);
				}
			}
		}
	}

	// Ensure that manually selected frames have associated actors and focal structures, even if their score is low
	for (auto& It : KeyMomentEvaluations)
	{
		FUpStageKeyMomentEvaluation& Eval = It.Value;

		if (Eval.Actors.Num() == 0)
		{
			for (const TPair<AActor*, FUpStageSequencerMovementAnalysis>& Pair : PerformerFrameActions)
			{
				AActor* Actor = Pair.Key;
				const FUpStageSequencerMovementAnalysis& MovementAnalysis = Pair.Value;

				int32 ArrayIndex = Eval.FrameIndex - PlaybackBounds.X;

				if (Pair.Key && Pair.Value.FrameFocalStructures.IsValidIndex(ArrayIndex))
				{
					Eval.Actors.Add(Pair.Key);
					Eval.FocalStructures.Add(Pair.Value.FrameFocalStructures[ArrayIndex]);
					UE_LOG(LogTemp, Warning, TEXT("Manually selected frame %d had no actors with score above threshold. Added actor %s for visibility."), Eval.FrameIndex, *Pair.Key->GetName());
				}
			}

			if (Eval.Actors.Num() == 0)
			{
				UE_LOG(LogTemp, Error, TEXT("CRITICAL: Frame %d finished fallback loop but still has 0 actors! It will be exported empty."), Eval.FrameIndex);
			}
		}
	}

	KeyMomentEvaluations.GenerateValueArray(KeyMomentsArray);
	return KeyMomentsArray;
}

TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::SelectKeyMoments(const TArray<FUpStageKeyMomentEvaluation>& KeyMomentEvaluations, const FUpStageSequencerKeyMomentParameter& Parameter)
{
	TArray<FUpStageKeyMomentEvaluation> SortedEvaluations = KeyMomentEvaluations;
	SortedEvaluations.Sort([](const FUpStageKeyMomentEvaluation& A, const FUpStageKeyMomentEvaluation& B) {
		if (A.bIsManualOverride != B.bIsManualOverride) return A.bIsManualOverride;

		return A.KeyMomentScore > B.KeyMomentScore;
	});

	if (Parameter.bUseNonMaximumSupression) SortedEvaluations = NonMaximumSuppression(SortedEvaluations, Parameter.MinFrameDistance);
	TArray<FUpStageKeyMomentEvaluation> FinalEvaluations;
	int32 AutoFramesKept = 0;

	for (const FUpStageKeyMomentEvaluation& Eval : SortedEvaluations)
	{
		if (Eval.bIsManualOverride)
		{
			FinalEvaluations.Add(Eval);
		}
		else if (AutoFramesKept < Parameter.TopN)
		{
			FinalEvaluations.Add(Eval);
			AutoFramesKept++;
		}
	}

	return FinalEvaluations;
}

TArray<FUpStageKeyMomentEvaluation> UUpStageAnalysisFunctions::NonMaximumSuppression(const TArray<FUpStageKeyMomentEvaluation>& SortedKeyMoments, int32 MinFrameDistance)
{
	TArray<FUpStageKeyMomentEvaluation> FilteredMoments;
	for (const FUpStageKeyMomentEvaluation& CurrentEval : SortedKeyMoments)
	{
		if (CurrentEval.bIsManualOverride)
		{
			FilteredMoments.Add(CurrentEval);
			continue;
		}

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
		float CompositionScore = CalculateCompositionScore(Camera, KeyMoment.FocalStructures, Parameter.CameraSelectionParameter);
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

float UUpStageAnalysisFunctions::CalculateCompositionScore(ACineCameraActor* Camera, const TArray<FUpStagePerformerFocalStructure>& FocalStructures, const FUpStageCameraSelectionParameter& Parameter)
{
	if (!Camera || FocalStructures.IsEmpty()) return 0.f;

	UCineCameraComponent* CameraComp = Camera->GetCineCameraComponent();
	if (!CameraComp) return 0.f;

	FTransform CameraTransform = Camera->GetActorTransform();
	float FOV = CameraComp->FieldOfView;
	float AspectRatio = CameraComp->AspectRatio;

	float TanHalfFOV = FMath::Tan(FMath::DegreesToRadians(FOV * 0.5f));
	float TotalCompositionScore = 0.f;

	for (const FUpStagePerformerFocalStructure& FocalStruct : FocalStructures)
	{
		FVector LocalLoc = CameraTransform.InverseTransformPosition(FocalStruct.MainFocalTransform.GetLocation());
		if (LocalLoc.X <= 0.f)
		{
			TotalCompositionScore -= Parameter.CompositionScore;
			continue;
		}

		float ScreenX = LocalLoc.Y / (LocalLoc.X * TanHalfFOV);
		float ScreenY = LocalLoc.Z / (LocalLoc.X * (TanHalfFOV / AspectRatio));

		if (FMath::Abs(ScreenX) > 1.0f || FMath::Abs(ScreenY) > 1.0f)
		{
			TotalCompositionScore -= (Parameter.CompositionScore * Parameter.CompositionOffScreenPenalty);
			continue;
		}
		else if (FMath::Abs(ScreenX) > Parameter.CompositionEdgeScreen || FMath::Abs(ScreenY) > Parameter.CompositionEdgeScreen)
		{
			TotalCompositionScore -= (Parameter.CompositionScore * Parameter.CompositionEdgeScreenPenalty);
		}

		float DistToThirdX = FMath::Min(FMath::Abs(ScreenX - 0.333f), FMath::Abs(ScreenX + 0.333f));
		float ThirdsScoreX = 1.0f - FMath::Clamp(DistToThirdX * 1.5f, 0.0f, 1.0f);

		float HeadroomScore = 1.0f - FMath::Clamp(FMath::Abs(ScreenY - 0.333f) * 2.0f, 0.0f, 1.0f);

		TotalCompositionScore += Parameter.CompositionScore * (ThirdsScoreX * Parameter.CompositionRuleOfThirdsPortion + HeadroomScore * Parameter.CompositionHeadroomPortion);

		if (!Parameter.bCheckFocalMemberComposition) continue;

		for (const FUpStagePerformerFocalMember& Member : FocalStruct.FocalMembers)
		{
			FVector MemberLocal = CameraTransform.InverseTransformPosition(Member.FocalMemberTransform.GetLocation());
			if (MemberLocal.X > 0.f)
			{
				float MemScreenX = MemberLocal.Y / (MemberLocal.X * TanHalfFOV);
				float MemScreenY = MemberLocal.Z / (MemberLocal.X * (TanHalfFOV / AspectRatio));

				if (FMath::Abs(MemScreenX) <= Parameter.CompositionSafeScreen && FMath::Abs(MemScreenY) <= Parameter.CompositionSafeScreen)
				{
					TotalCompositionScore += (Parameter.CompositionScore * Parameter.CompositionFocalMemberSafeBoost * Member.ImportanceScore);
				}
			}
		}
	}

	return TotalCompositionScore;
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

TArray<FUpStageCameraCut> UUpStageAnalysisFunctions::StitchCameraCuts(
	const TArray<FUpStageCameraCut>& CameraCuts, 
	const FIntPoint& FrameBounds,
	const FUpStageSequencerCameraCutsParameter& Parameter)
{
	TArray<FUpStageCameraCut> StitchedCuts;
	if (CameraCuts.IsEmpty()) return StitchedCuts;

	TArray<FUpStageCameraCut> SortedCuts = CameraCuts;
	SortedCuts.Sort([](const FUpStageCameraCut& A, const FUpStageCameraCut& B) {
		return A.FrameStart < B.FrameStart;
	});

	int32 BoundX = FrameBounds.X;
	int32 BoundY = FrameBounds.Y;

	bool bHasPrevious = false;
	FUpStageCameraCut PreviousCut;

	for (const FUpStageCameraCut& CurrentCut : SortedCuts)
	{
		if (!bHasPrevious)
		{
			if (CurrentCut.FrameStart > BoundX)
			{
				FUpStageCameraCut InitialGap;
				InitialGap.Camera = nullptr;
				InitialGap.FrameStart = BoundX;
				InitialGap.FrameEnd = CurrentCut.FrameStart;
				StitchedCuts.Add(InitialGap);
			}

			PreviousCut = CurrentCut;
			bHasPrevious = true;
		}
		else
		{
			if (CurrentCut.FrameStart <= PreviousCut.FrameEnd + Parameter.CameraGenerationParameter.MinFrameDistance)
			{
				if (CurrentCut.Camera == PreviousCut.Camera)
				{
					PreviousCut.FrameEnd = FMath::Max(PreviousCut.FrameEnd, CurrentCut.FrameEnd);
				}
				else
				{
					FUpStageCameraCut NewCut = CurrentCut;

					if (Parameter.CameraGenerationParameter.bOverlapCuts)
					{
						int32 OverlapFrame = PreviousCut.FrameEnd;
						PreviousCut.FrameEnd = CurrentCut.FrameStart;
						NewCut.FrameStart = OverlapFrame;
					}
					else
					{
						int32 MiddleFrame = PreviousCut.FrameEnd + ((CurrentCut.FrameStart - PreviousCut.FrameEnd) / 2);
						PreviousCut.FrameEnd = MiddleFrame;
						NewCut.FrameStart = MiddleFrame;
					}

					StitchedCuts.Add(PreviousCut);
					PreviousCut = NewCut;
				}
			}
			else
			{
				StitchedCuts.Add(PreviousCut);

				FUpStageCameraCut MidGap;
				MidGap.Camera = nullptr;
				MidGap.FrameStart = PreviousCut.FrameEnd;
				MidGap.FrameEnd = CurrentCut.FrameStart;
				StitchedCuts.Add(MidGap);

				PreviousCut = CurrentCut;
			}
		}
	}

	if(bHasPrevious) StitchedCuts.Add(PreviousCut);

	if (StitchedCuts.Last().FrameEnd < BoundY)
	{
		FUpStageCameraCut FinalGap;
		FinalGap.Camera = nullptr;
		FinalGap.FrameStart = StitchedCuts.Last().FrameEnd;
		FinalGap.FrameEnd = BoundY;
		StitchedCuts.Add(FinalGap);
	}

	switch (Parameter.CameraGenerationParameter.CutFillMethod)
	{
		case EUpStageCameraCutFillMethod::Fallback:
			StitchedCuts = FillCameraCutWithFallback(StitchedCuts, Parameter);
			break;
		case EUpStageCameraCutFillMethod::Neighbor:
			StitchedCuts = FillCameraCutWithNeighbor(StitchedCuts, Parameter);
			break;
		default:
			break;
	}

	return StitchedCuts;
}

TArray<FUpStageCameraCut> UUpStageAnalysisFunctions::FillCameraCutWithFallback(
	const TArray<FUpStageCameraCut>& CameraCuts, 
	const FUpStageSequencerCameraCutsParameter& Parameter)
{
	TArray<FUpStageCameraCut> StitchedCuts = CameraCuts;
	for (int32 i = 0; i < StitchedCuts.Num(); ++i)
	{
		if (StitchedCuts[i].Camera == nullptr)
		{
			StitchedCuts[i].Camera = Parameter.CameraGenerationParameter.FallbackCamera;
		}
	}

	return StitchedCuts;
}

TArray<FUpStageCameraCut> UUpStageAnalysisFunctions::FillCameraCutWithNeighbor(
	const TArray<FUpStageCameraCut>& CameraCuts, 
	const FUpStageSequencerCameraCutsParameter& Parameter)
{
	TArray<FUpStageCameraCut> StitchedCuts = CameraCuts;
	// Forward pass to fill gaps with previous cameras
	for (int32 i = 0; i < StitchedCuts.Num(); ++i)
	{
		if (StitchedCuts[i].Camera == nullptr && i > 0 && StitchedCuts[i - 1].Camera != nullptr)
		{
			StitchedCuts[i].Camera = StitchedCuts[i - 1].Camera;
		}
	}

	// Backward pass to fill any remaining gaps
	for (int32 i = StitchedCuts.Num() - 2; i >= 0; --i)
	{
		if (StitchedCuts[i].Camera == nullptr && StitchedCuts[i + 1].Camera != nullptr)
		{
			StitchedCuts[i].Camera = StitchedCuts[i + 1].Camera;
		}
	}
	return StitchedCuts;
}

float UUpStageAnalysisFunctions::CalculateKeyMomentFocusDistance(ACineCameraActor* Camera, const FUpStageKeyMomentEvaluation& KeyMoment)
{
	if (!Camera || KeyMoment.FocalStructures.IsEmpty()) return 0.f;

	FVector CameraLocation = Camera->GetActorLocation();
	FVector CameraForward = Camera->GetActorForwardVector();

	float MinDepth = TNumericLimits<float>::Max();
	float MaxDepth = -TNumericLimits<float>::Max();
	bool bFoundValidTarget = false;

	for (const FUpStagePerformerFocalStructure& FocalStruct : KeyMoment.FocalStructures)
	{
		FVector TargetLocation = FocalStruct.MainFocalTransform.GetLocation();
		FVector DirToTarget = TargetLocation - CameraLocation;

		float ProjectedDepth = FVector::DotProduct(DirToTarget, CameraForward);

		if (ProjectedDepth > 0.f)
		{
			if (ProjectedDepth < MinDepth) MinDepth = ProjectedDepth;
			if (ProjectedDepth > MaxDepth) MaxDepth = ProjectedDepth;
			bFoundValidTarget = true;
		}
	}

	if (!bFoundValidTarget) return 0.f;
	return (MinDepth + MaxDepth) * 0.5f;
}