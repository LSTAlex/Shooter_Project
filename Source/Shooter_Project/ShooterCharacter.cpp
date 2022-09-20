// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Containers/UnrealString.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//—оздаЄт CameraBoom (прит€гиваетс€ к персонажу, если есть коллизии)
	//Create CameraBoom (pulls in towards the character if the is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//—ледует за персонажем на этом рассто€нии
	//The camera foolows at this distance behind the character
	CameraBoom->TargetArmLength = 300.f;
	//вращает рычаг, основыва€сь на вращении контроллера
	//Rotate the arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	float print{ 412.f };
	
	UE_LOG(LogTemp, Warning, TEXT("Hi,%f"), print);

}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

