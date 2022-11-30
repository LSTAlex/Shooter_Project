// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_PROJECT_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AShooterPlayerController();

protected:

	virtual void BeginPlay() override;

private:

	//Ссылка на класс общего оперлея HUD
	//Reference to the overall HUD overlay BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDOverlayClass;

	//Переменная, которая содержит widget HUD overlay
	//Variable to holds the HUD overlay widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay;
};
