// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CB_PlayerUIWidget.generated.h"

class UButton;
class UTextBlock;
class UPanelWidget;
/**
 * 
 */
UCLASS()
class PROJECTCB_API UCB_PlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UCB_PlayerUIWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TeamAliveText;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* EnemyAliveText;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* BallsCapturedText;

	void SetTeamAliveTextBlock(FString TextToSet);
	void SetEnemyAliveTextBlock(FString TextToSet);
	void SetBallsCaptuedTextBlock(FString TextToSet);

protected:

	virtual void NativeConstruct() override;

};
