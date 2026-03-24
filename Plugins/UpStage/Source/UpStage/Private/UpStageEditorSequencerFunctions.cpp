// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageEditorSequencerFunctions.h"

void UUpStageEditorSequencerFunctions::AddKeyframeToFloatProperty(ULevelSequence* InSequence, FMovieSceneObjectBindingID ObjectBindingID, FName PropertyName, FString PropertyPath, int32 FrameIndex, float Value)
{
	FGuid BindingID = ObjectBindingID.GetGuid();
	if (!InSequence || !BindingID.IsValid()) return;

	UMovieScene* MovieScene = InSequence->GetMovieScene();
	if (!MovieScene) return;

	FFrameRate DisplayRate = MovieScene->GetDisplayRate();
	FFrameRate TickResolution = MovieScene->GetTickResolution();

	FFrameTime TickTime = ConvertFrameTime(FFrameTime(FrameIndex), DisplayRate, TickResolution);
	FFrameNumber KeyTime = TickTime.FloorToFrame();

	UMovieSceneFloatTrack* FloatTrack = nullptr;
    for (UMovieSceneTrack* Track : MovieScene->FindTracks(UMovieSceneFloatTrack::StaticClass(), BindingID))
    {
        UMovieSceneFloatTrack* PropTrack = Cast<UMovieSceneFloatTrack>(Track);
        if (PropTrack && PropTrack->GetPropertyPath() == PropertyPath)
        {
            FloatTrack = PropTrack;
            break;
        }
	}

    if (!FloatTrack)
    {
        FloatTrack = MovieScene->AddTrack<UMovieSceneFloatTrack>(BindingID);
        FloatTrack->SetPropertyNameAndPath(PropertyName, PropertyPath);
	}

    UMovieSceneFloatSection* FloatSection = nullptr;
    if (FloatTrack->GetAllSections().Num() > 0)
    {
        FloatSection = Cast<UMovieSceneFloatSection>(FloatTrack->GetAllSections()[0]);
    }
    else
    {
        FloatSection = Cast<UMovieSceneFloatSection>(FloatTrack->CreateNewSection());
        FloatTrack->AddSection(*FloatSection);
        FloatSection->SetRange(TRange<FFrameNumber>::All());
	}

    FMovieSceneChannelProxy& ChannelProxy = FloatSection->GetChannelProxy();
    TArrayView<FMovieSceneFloatChannel*> FloatChannels = ChannelProxy.GetChannels<FMovieSceneFloatChannel>();
    if (FloatChannels.IsValidIndex(0))
    {
        FMovieSceneFloatChannel* Channel = FloatChannels[0];
        Channel->GetData().AddKey(KeyTime.Value, FMovieSceneFloatValue(Value));
        Channel->AutoSetTangents();
	}

	InSequence->MarkPackageDirty();
}

void UUpStageEditorSequencerFunctions::DeleteKeyframesFromFloatProperty(ULevelSequence* InSequence, FMovieSceneObjectBindingID ObjectBindingID, FName PropertyName, FString PropertyPath)
{
    FGuid BindingID = ObjectBindingID.GetGuid();
    if (!InSequence || !BindingID.IsValid()) return;

    UMovieScene* MovieScene = InSequence->GetMovieScene();
    if (!MovieScene) return;

    UMovieSceneTrack* TrackToRemove = nullptr;

    for (UMovieSceneTrack* Track : MovieScene->FindTracks(UMovieSceneFloatTrack::StaticClass(), BindingID))
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
        InSequence->MarkPackageDirty();
    }
}

int32 UUpStageEditorSequencerFunctions::AddCustomColoredMarkedFrame(UMovieSceneSequence* InSequence, const FMovieSceneMarkedFrame& InMarkedFrame, FColor MarkColor, EMovieSceneTimeUnit TimeUnit)
{
    if (!InSequence) return INDEX_NONE;

    UMovieScene* MovieScene = InSequence->GetMovieScene();
    if (!MovieScene) return INDEX_NONE;

    FMovieSceneMarkedFrame NewMarkedFrame = InMarkedFrame;
    NewMarkedFrame.bUseCustomColor = true;
    NewMarkedFrame.CustomColor = MarkColor;

    if (TimeUnit == EMovieSceneTimeUnit::DisplayRate)
    {
        FFrameRate DisplayRate = MovieScene->GetDisplayRate();
        FFrameRate TickResolution = MovieScene->GetTickResolution();

        FFrameTime InternalTime = FFrameRate::TransformTime(FFrameTime(NewMarkedFrame.FrameNumber), DisplayRate, TickResolution);
        NewMarkedFrame.FrameNumber = InternalTime.FrameNumber;
    }

    int32 NewIndex = MovieScene->AddMarkedFrame(NewMarkedFrame);

    InSequence->MarkPackageDirty();

    return NewIndex;
}