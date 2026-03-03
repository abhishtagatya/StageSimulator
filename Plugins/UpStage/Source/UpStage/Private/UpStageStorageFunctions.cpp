// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageStorageFunctions.h"

void UUpStageStorageFunctions::TrackActorMovementAnalysis(
    TMap<AActor*, FUpStageSequencerMovementAnalysis>& Map,
    AActor* Actor,
    const FUpStageFrameMovementAnalysis& FrameAnalysis)
{
    if (!Actor) return;

    FUpStageSequencerMovementAnalysis& MovementAnalysis =
        Map.FindOrAdd(Actor);

    MovementAnalysis.FrameMovementAnalyses.Add(FrameAnalysis);
}