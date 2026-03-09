// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpStageFrameMovementAnalysis.h"
#include "UpStageSequencerMovementAnalysis.h"
#include "UpStageSequencerKeyMomentParameter.h"
#include "UpStageKeyMomentEvaluation.h"
#include "UpStageLabanEffortActionDimension.h"
#include "UpStageLabanEffortExtremities.h"
#include "UpStageExtremityStructure.h"
#include "UpStageSequencerCameraCutsParameter.h"
#include "UpStageCameraSelectionParameter.h"
#include "UpStageCameraCut.h"
#include "CineCameraActor.h"
#include "UpStageAnalysisFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UPSTAGE_API UUpStageAnalysisFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static TArray<FUpStageKeyMomentEvaluation> ExtractKeyMoments(const TMap<AActor*, FUpStageSequencerMovementAnalysis>& PerformerFrameActions, const FUpStageSequencerKeyMomentParameter& Parameter);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static TArray<FUpStageKeyMomentEvaluation> ExtractKeyMomentsFromSelectedFrames(const TMap<AActor*, FUpStageSequencerMovementAnalysis>& PerformerFrameActions, const FUpStageSequencerKeyMomentParameter& Parameter, const TArray<int32>& Frames);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Selection")
	static TArray<FUpStageKeyMomentEvaluation> SelectKeyMoments(const TArray<FUpStageKeyMomentEvaluation>& KeyMomentEvaluations, const FUpStageSequencerKeyMomentParameter& Parameter);
	
	UFUNCTION(BlueprintCallable, Category = "UpStage|Selection")
	static ACineCameraActor* SelectBestCameraForKeyMoment(const TArray<ACineCameraActor*>& CameraArray, const FUpStageKeyMomentEvaluation& KeyMoment, const FUpStageSequencerCameraCutsParameter& Parameter);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Selection")
	static EUpStageLabanEffortAction TransitionEffortAction(EUpStageLabanEffortAction EffortAction, FUpStageFrameMovementAnalysis FrameMovementAnalysis, FVector2D Threshold);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static float CalculateEffortTransitionScore(EUpStageLabanEffortAction PreviousEffortAction, EUpStageLabanEffortAction CurrentEffortAction, const FUpStageActionTransitionAnalysisParameter& Parameter);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static float CalculateEffortIntensity(const FUpStageFrameMovementAnalysis& FrameMovementAnalysis);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static float CalculateEffortExtremityScore(const FUpStageFrameMovementAnalysis& FrameMovementAnalysis, FUpStageLabanEffortExtremities& EffortExtremtities, const FUpStageExtremityAnalysisParameter& Parameter);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static float CalculateAlignmentScore(ACineCameraActor* Camera, FVector Centroid, FVector Forward, const FUpStageCameraSelectionParameter& Parameter);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static float CalculateVisibilityScore(ACineCameraActor* Camera, const TArray<FUpStagePerformerFocalStructure>& FocalStructures, const FUpStageCameraSelectionParameter& Parameter);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static float CalculateCompositionScore(ACineCameraActor* Camera, const TArray<FUpStagePerformerFocalStructure>& FocalStructures, const FUpStageCameraSelectionParameter& Parameter);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static TArray<FUpStageCameraCut> StitchCameraCuts(const TArray<FUpStageCameraCut>& CameraCuts, const FVector2D& FrameBounds, const FUpStageSequencerCameraCutsParameter& Parameter);

private:
	static FUpStageLabanEffortActionDimension GetEffortDimensions(EUpStageLabanEffortAction Action);
	static EUpStageLabanEffortAction GetEffortAction(FUpStageLabanEffortActionDimension Dimension);
	static bool ExtremityDetection(float CurrentValue, FUpStageExtremityStructure& PrevExtremity, float Tolerance);

	static TArray<FUpStageKeyMomentEvaluation> NonMaximumSuppression(const TArray<FUpStageKeyMomentEvaluation>& KeyMomentEvaluations, int32 MinFrameDistance);

	static TArray<FUpStageCameraCut> FillCameraCutWithFallback(const TArray<FUpStageCameraCut>& CameraCuts, const FUpStageSequencerCameraCutsParameter& Parameter);
	static TArray<FUpStageCameraCut> FillCameraCutWithNeighbor(const TArray<FUpStageCameraCut>& CameraCuts, const FUpStageSequencerCameraCutsParameter& Parameter);
};
