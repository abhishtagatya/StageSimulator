#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConsoleVariablesAsset.h"
#include "UpStageSequencerRenderQueueParameter.generated.h"


USTRUCT(BlueprintType)
struct UPSTAGE_API FUpStageSequencerRenderQueueParameter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Queue")
	EUpStageRenderQueueOutputFormat OutputFormat = EUpStageRenderQueueOutputFormat::StillsPNG;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Queue")
	FString OutputDirectory = FPaths::ProjectSavedDir() / TEXT("UpStageRenders");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Queue")
	FString FileNamePrefix = TEXT("UpStage_Render");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Queue")
	bool bOverwriteExistingFiles = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Queue")
	FIntPoint OutputResolution = FIntPoint(1920, 1080);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Render Queue")
	UConsoleVariablesAsset* OverrideConsoleVariables = nullptr;

};