// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpStageFrameMovementAnalysis.h"
#include "UpStageSequencerMovementAnalysis.h"
#include "UpStageSequencerKeyMomentParameter.h"
#include "UpStageKeyMomentEvaluation.h"
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

	UFUNCTION(BlueprintCallable, Category = "UpStage|Selection")
	static TArray<FUpStageKeyMomentEvaluation> SelectKeyMoments(const TArray<FUpStageKeyMomentEvaluation>& KeyMomentEvaluations, int32 TopN);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Selection")
	static EUpStageLabanEffortAction TransitionEffortAction(EUpStageLabanEffortAction EffortAction, FUpStageFrameMovementAnalysis FrameMovementAnalysis, FVector2D Threshold);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Analysis")
	static int CalculateEffortTransitionScore(EUpStageLabanEffortAction PreviousEffortAction, EUpStageLabanEffortAction CurrentEffortAction, const FUpStageActionTransitionAnalysisParameter& Parameter);
};
