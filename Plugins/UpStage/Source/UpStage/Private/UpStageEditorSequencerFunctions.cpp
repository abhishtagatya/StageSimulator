// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageEditorSequencerFunctions.h"

void UUpStageEditorSequencerFunctions::AddFocusDistanceKeyframe(ULevelSequence* InSequence, FMovieSceneObjectBindingID CameraObjectBindingID, int32 FrameIndex, float FocusDistance)
{
    FGuid CameraBindingID = CameraObjectBindingID.GetGuid();
    if (!InSequence || !CameraBindingID.IsValid()) return;

    UMovieScene* MovieScene = InSequence->GetMovieScene();
    if (!MovieScene) return;

    FFrameRate DisplayRate = MovieScene->GetDisplayRate();
    FFrameRate TickResolution = MovieScene->GetTickResolution();

    FFrameTime TickTime = ConvertFrameTime(FFrameTime(FrameIndex), DisplayRate, TickResolution);
    FFrameNumber KeyTime = TickTime.FloorToFrame();

    FName PropertyName("Manual Focus Distance");
    FString PropertyPath("FocusSettings.ManualFocusDistance");

    UMovieSceneFloatTrack* FocusTrack = nullptr;

    for (UMovieSceneTrack* Track : MovieScene->FindTracks(UMovieSceneFloatTrack::StaticClass(), CameraBindingID))
    {
        UMovieSceneFloatTrack* PropTrack = Cast<UMovieSceneFloatTrack>(Track);
        if (PropTrack && PropTrack->GetPropertyPath() == PropertyPath)
        {
            FocusTrack = PropTrack;
            break;
        }
    }

    if (!FocusTrack)
    {
        FocusTrack = MovieScene->AddTrack<UMovieSceneFloatTrack>(CameraBindingID);
        FocusTrack->SetPropertyNameAndPath(PropertyName, PropertyPath);
    }

    UMovieSceneFloatSection* FocusSection = nullptr;
    if (FocusTrack->GetAllSections().Num() > 0)
    {
        FocusSection = Cast<UMovieSceneFloatSection>(FocusTrack->GetAllSections()[0]);
    }
    else
    {
        FocusSection = Cast<UMovieSceneFloatSection>(FocusTrack->CreateNewSection());
        FocusTrack->AddSection(*FocusSection);

        FocusSection->SetRange(TRange<FFrameNumber>::All());
    }

    FMovieSceneChannelProxy& ChannelProxy = FocusSection->GetChannelProxy();
    TArrayView<FMovieSceneFloatChannel*> FloatChannels = ChannelProxy.GetChannels<FMovieSceneFloatChannel>();

    if (FloatChannels.IsValidIndex(0))
    {
        FMovieSceneFloatChannel* Channel = FloatChannels[0];

        Channel->GetData().AddKey(KeyTime.Value, FMovieSceneFloatValue(FocusDistance));
        Channel->AutoSetTangents();
    }

    InSequence->MarkPackageDirty();
}

void UUpStageEditorSequencerFunctions::DeleteAllFocusDistanceKeyframes(ULevelSequence* InSequence, const TArray<FMovieSceneObjectBindingID>& CameraObjectBindings)
{
    if (!InSequence) return;

    UMovieScene* MovieScene = InSequence->GetMovieScene();
    if (!MovieScene) return;

    bool bSequenceWasModified = false;
    FString PropertyPath("FocusSettings.ManualFocusDistance");

    for (const FMovieSceneObjectBindingID& Binding : CameraObjectBindings)
    {
        FGuid ComponentBindingID = Binding.GetGuid();
        if (!ComponentBindingID.IsValid()) continue;

        UMovieSceneTrack* TrackToRemove = nullptr;

        for (UMovieSceneTrack* Track : MovieScene->FindTracks(UMovieSceneFloatTrack::StaticClass(), ComponentBindingID))
        {
            UMovieSceneFloatTrack* PropTrack = Cast<UMovieSceneFloatTrack>(Track);
            if (PropTrack && PropTrack->GetPropertyPath() == PropertyPath)
            {
                TrackToRemove = PropTrack;
                break;
            }
        }

        if (TrackToRemove)
        {
            MovieScene->RemoveTrack(*TrackToRemove);
            bSequenceWasModified = true;
        }
    }

    if (bSequenceWasModified) InSequence->MarkPackageDirty();
}