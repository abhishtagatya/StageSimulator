// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelSequence.h"
#include "MovieScene.h"
#include "MovieSceneBindingProxy.h"
#include "Tracks/MovieSceneFloatTrack.h"
#include "Sections/MovieSceneFloatSection.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "CineCameraComponent.h"
#include "UpStageEditorSequencerFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UPSTAGE_API UUpStageEditorSequencerFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UpStage|Sequencer")
	static void AddKeyframeToFloatProperty(ULevelSequence* InSequence, FMovieSceneObjectBindingID ObjectBindingID, FName PropertyName, FString PropertyPath, int32 FrameIndex, float Value);

	UFUNCTION(BlueprintCallable, Category = "UpStage|Sequencer")
	static void DeleteKeyframesFromFloatProperty(ULevelSequence* InSequence, FMovieSceneObjectBindingID ObjectBindings, FName PropertyName, FString PropertyPath);
};
