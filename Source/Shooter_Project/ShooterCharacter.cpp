// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Containers/UnrealString.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	//скорости вращения в прицеле/ не в прицеле
	//turn rates for aiming/not aiming
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
	//множетели для обзора мышью
	//mouse look sensitivity scale factors
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.45f),
	MouseAimingLookUpRate(0.45f),
	//true - если прицеливается 
	//true when aiming the weapon
	bAiming(false),
	//настройки поля обзора
	//setting FOV camera
	CameraDefaultFOV(0.f),//set in BeginPlay
	CameraZoomedFOV(35.f),
	CameraCurrentFOV(0.f),
	ZoomInterpspeed(20.f)
	
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Создаёт CameraBoom (притягивается к персонажу, если есть коллизии)
	//Create CameraBoom (pulls in towards the character if the is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//Следует за персонажем на этом расстоянии
	//The camera foolows at this distance behind the character
	CameraBoom->TargetArmLength = 180.f;
	//вращает рычаг, основываясь на вращении контроллера
	//Rotate the arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	//создание следующей камеры
	//Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Прикрепление камеры к концу рычага
	// Attach camera to end of boom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//Камера не вращается относительно рычага
	//Camera does not rotate relative to arm
	FollowCamera->bUsePawnControlRotation = false;

	//не вращается при вращении контроллера
	//don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//настройка движения персонажа
	//Configurate character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);//скорость вращения
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
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
	//рассчитать дельта время для этого кадра из информации о скорости
	//calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	//рассчитать дельта время для этого кадра из информации о скорости
	//calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Turn(float Value)
{
	float TurnScaleFactor{};
	if (bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Value)
{
	float LookUpScaleFactor{};
	if (bAiming)
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}
	AddControllerPitchInput(Value * LookUpScaleFactor);
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

		//get end beam
		FVector BeamEnd;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);
		if (bBeamEnd)
		{
			//конечная точка дымного следа - местоположение попадания трассировки
			//Beam end point is now trace hit location
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					BeamEnd);
			}

			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				BeamParticles,
				SocketTransform);
			//Существуют ли частицы дымного следа
			//Are smoke trail particles creatures
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
}

void AShooterCharacter::CameraInterpZoom(float DeltaTime)
{
	//нажата ли кнопка прицеливания?
	//устанавливает текущее поле обзора камеры
	//Aiming button pressed?
	//set current camera FOV
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpspeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpspeed);
	}
	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
}

void AShooterCharacter::SetLookRates()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AShooterCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	//скорость персонажа по-умолчанию 600
	//speed character default 600
	FVector2D WalkSpeedRange{ 0.f,600.f };
	FVector2D VelocityMultiplier{ 0.f,1.f };
	FVector Velocity{ GetVelocity() };
	//необходимы значения ускарения только X и Y
	//need speen only vector direction X and Y
	Velocity.Z = 0;

	//нормализация скорости персонажа
	//normalize speed character
	//600 = 1, 300 = 0.5
	CrosshairVelosityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange, 
		VelocityMultiplier, 
		Velocity.Size());

	//вычисление фактора нахождения в воздухе для прицела
	//Calculate croshair in air factor
	if (GetCharacterMovement()->IsFalling())
	{
		//расширение перекрестия, пока персонаж в воздухе
		//Spread the crosshair while in air
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.f);
	}
	else
	{
		//быстро сжимает прицел, когда персонаж находится на земле
		//Shrink the crosshairs rapidly while on yhe ground
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 20.f);
	}

	CrosshairSpreadMultiplier = 0.5f + 
		CrosshairVelosityFactor + 
		CrosshairInAirFactor;
}

bool AShooterCharacter::GetBeamEndLocation(
	const FVector& MuzzelSocketLocation, 
	FVector& OutBeamLocation)
{
	//получение текущего размера Viewport
		//get current size of the Viewport
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	//Местоположение перекрестия в экранном пространстве
	//Get screen space location of crosshairs
	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	CrosshairLocation.Y -= 50.f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	//Получение мирового положения и направления для перекрестия
	//Get world position and direction of crosshair
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

    //была ли дипроекция успешна
	//was deprojection successful
	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f };

		//установление конечной точки дымного следа как конечная точка трассировки
		//Set beam end point to line trace end pint
		OutBeamLocation = End;

		//трассировка вперед от мирового положения перекрестия
		//Trace outward from crosshairs world location
		GetWorld()->LineTraceSingleByChannel(
			ScreenTraceHit,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);

		//было ли попадание трассировки
		//was there a trace hit
		if (ScreenTraceHit.bBlockingHit)
		{
			OutBeamLocation = ScreenTraceHit.Location;
		}

		//2 трассировка от ствола оружия
		//Perform a second trace. this time from the gun barrel
		FHitResult WeaponTraceHit;
		const FVector WeaponTraceStart{ MuzzelSocketLocation };
		const FVector WeaponTraceEnd{ OutBeamLocation };
		GetWorld()->LineTraceSingleByChannel(
			WeaponTraceHit,
			WeaponTraceStart,
			WeaponTraceEnd,
			ECollisionChannel::ECC_Visibility);

		//есть ли объект между стволом и конечной точкой дымного следа
		//Object between barrel and 
		if (WeaponTraceHit.bBlockingHit)
		{
			OutBeamLocation = WeaponTraceHit.Location;
		}

		return true;
	}

	return false;
}

//устанавливает значение bAiming в зависимости от нажатия кнопки
//Set bAiming to true or false with button
void AShooterCharacter::AimingButtonPressed()
{
	bAiming = true;
}

void AShooterCharacter::AimingButtonReleased()
{
	bAiming = false;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraInterpZoom(DeltaTime);

	//изменение чувствительности вращения в зависимости от прицеливания
	//Change look sensitivity based on aiming
	SetLookRates();
	
	//вычисление множителя расширения перекрестия
	//Calculate crosshair spread multuplier
	CalculateCrosshairSpread(DeltaTime);
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
	PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this, 
		&AShooterCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, 
		&AShooterCharacter::StopJumping);
	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, 
		&AShooterCharacter::FireWeapon);
	PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this,
		&AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", IE_Released, this,
		&AShooterCharacter::AimingButtonReleased);
}

