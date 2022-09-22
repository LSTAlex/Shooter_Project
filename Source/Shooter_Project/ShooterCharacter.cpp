// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Containers/UnrealString.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"


// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f)
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

	//создание следующей камеры
	//Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// ѕрикрепление камеры к концу рычага
	// Attach camera to end of boom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// амера не вращаетс€ относительно рычага
	//Camera does not rotate relative to arm
	FollowCamera->bUsePawnControlRotation = false;

	//не вращаетс€ при вращении контроллера
	//don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//настройка движени€ персонажа
	//Configurate character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);//скорость вращени€
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		//find out which way is forward
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };

		const FVector Direction { FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value); 
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		//find out which way is forward
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	//рассчитать дельта врем€ дл€ этого кадра из информации о скорости
	//calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	//рассчитать дельта врем€ дл€ этого кадра из информации о скорости
	//calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::FireWeapon()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this,FireSound);
	}

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzelFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzelFlash, SocketTransform);
		}
	}
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
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this, 
		&AShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, 
		&AShooterCharacter::StopJumping);
	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, 
		&AShooterCharacter::FireWeapon);

}

