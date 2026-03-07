// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageStorageFunctions.h"

void UUpStageStorageFunctions::TrackActorMovementAnalysis(
    TMap<AActor*, FUpStageSequencerMovementAnalysis>& Map,
    AActor* Actor,
    const FUpStageFrameMovementAnalysis& FrameAnalysis,
    const FUpStagePerformerFocalStructure& FocalStructure)
{
    if (!Actor) return;

    FUpStageSequencerMovementAnalysis& MovementAnalysis =
        Map.FindOrAdd(Actor);

    MovementAnalysis.FrameMovementAnalyses.Add(FrameAnalysis);
    MovementAnalysis.FrameFocalStructures.Add(FocalStructure);
}