// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpStageFrameMovementAnalysis.h"
#include "UpStageSequencerMovementAnalysis.h"
#include "UpStagePerformerFocalStructure.h"
#include "UpStageStorageFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UPSTAGE_API UUpStageStorageFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UpStage|Storage")
	static void TrackActorMovementAnalysis(
		UPARAM(ref) TMap<AActor*, FUpStageSequencerMovementAnalysis>& Map,
		AActor* Actor,
		const FUpStageFrameMovementAnalysis& FrameAnalysis,
		const FUpStagePerformerFocalStructure& FocalStructure
	);
	
};
