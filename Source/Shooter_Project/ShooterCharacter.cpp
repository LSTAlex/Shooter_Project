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
#include "Item.h"
#include "Components/WidgetComponent.h"


// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	//�������� �������� � �������/ �� � �������
	//turn rates for aiming/not aiming
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
	//��������� ��� ������ �����
	//mouse look sensitivity scale factors
	MouseHipTurnRate(1.0f),
	MouseHipLookUpRate(1.0f),
	MouseAimingTurnRate(0.45f),
	MouseAimingLookUpRate(0.45f),
	//true - ���� ������������� 
	//true when aiming the weapon
	bAiming(false),
	//��������� ���� ������
	//setting FOV camera
	CameraDefaultFOV(0.f),//set in BeginPlay
	CameraZoomedFOV(35.f),
	CameraCurrentFOV(0.f),
	ZoomInterpspeed(20.f),
	//������� ���������� �����������
	//Crosshair spread factors
	CrosshairSpreadMultiplier(0.f),
	CrosshairVelosityFactor(0.f),
	CrosshairInAirFactor(0.f),
	CrosshairInAimFactor(0.f),
	CrosshairShootingFactor(0.f),
	//���������� ������� ��������
	//bullet fire timer variables
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	//���������� �������������� ��������
	//Automatic fire variable
	AutomaticFireRate(0.1f),
	bShouldFire(true),
	bFireButtonPressed(false),
	//���������� ����������� ���������
	//Item trace variable
	bShouldTraceForItems(false)
	
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������ CameraBoom (������������� � ���������, ���� ���� ��������)
	//Create CameraBoom (pulls in towards the character if the is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//������� �� ���������� �� ���� ����������
	//The camera foolows at this distance behind the character
	CameraBoom->TargetArmLength = 180.f;
	//������� �����, ����������� �� �������� �����������
	//Rotate the arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	//�������� ��������� ������
	//Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// ������������ ������ � ����� ������
	// Attach camera to end of boom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//������ �� ��������� ������������ ������
	//Camera does not rotate relative to arm
	FollowCamera->bUsePawnControlRotation = false;

	//�� ��������� ��� �������� �����������
	//don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//��������� �������� ���������
	//Configurate character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);//�������� ��������
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
	//���������� ������ ����� ��� ����� ����� �� ���������� � ��������
	//calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	//���������� ������ ����� ��� ����� ����� �� ���������� � ��������
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
			//�������� ����� ������� ����� - �������������� ��������� �����������
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
			//���������� �� ������� ������� �����
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

	//��������� ������ BulletFire ��� �����������
	//Start BulletFire timer for crosshair
	StartCrosshairBulletFire();
}

void AShooterCharacter::CameraInterpZoom(float DeltaTime)
{
	//������ �� ������ ������������?
	//������������� ������� ���� ������ ������
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
	//�������� ��������� ��-��������� 600
	//speed character default 600
	FVector2D WalkSpeedRange{ 0.f,600.f };
	FVector2D VelocityMultiplier{ 0.f,1.f };
	FVector Velocity{ GetVelocity() };
	//���������� �������� ��������� ������ X � Y
	//need speen only vector direction X and Y
	Velocity.Z = 0;

	//������������ �������� ���������
	//normalize speed character
	//600 = 1, 300 = 0.5
	CrosshairVelosityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange, 
		VelocityMultiplier, 
		Velocity.Size());

	//���������� ������� ���������� � ������� ��� �������
	//Calculate croshair in air factor
	if (GetCharacterMovement()->IsFalling())
	{
		//���������� �����������, ���� �������� � �������
		//Spread the crosshair while in air
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.f);
	}
	else
	{
		//������ ������� ������, ����� �������� ��������� �� �����
		//Shrink the crosshairs rapidly while on yhe ground
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 20.f);
	}

	//������������� �� ��?
	//are we aiming?
	if (bAiming)
	{
		//��������� ������ ����������� ����� ������
		//Shrink crosshair a small very quickly
		CrosshairInAimFactor = FMath::FInterpTo(
			CrosshairInAimFactor, 
			0.6f,
			DeltaTime, 
			30.0f);
	}
	else
	{
		//����� ������ ���������� ������������ � ���������� ���������
		//Spread crosshairs back to normal very quickly
		CrosshairInAimFactor = FMath::FInterpTo(
			CrosshairInAimFactor,
			0.0f,
			DeltaTime,
			30.0f);
	}


	if (bFiringBullet)
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.3f, DeltaTime, 60.f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 60.f);
	}

	CrosshairSpreadMultiplier = 0.5f + 
		CrosshairVelosityFactor + 
		CrosshairInAirFactor - 
		CrosshairInAimFactor +
		CrosshairShootingFactor;
}

void AShooterCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

void AShooterCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;
	
	GetWorldTimerManager().SetTimer(
		CrosshairShootTimer,
		this,
		&AShooterCharacter::FinishCrosshairBulletFire,
		ShootTimeDuration);
}

void AShooterCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	StartFireTimer();
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AShooterCharacter::StartFireTimer()
{
	if (bShouldFire)
	{
		FireWeapon();
		bShouldFire = false;
		GetWorldTimerManager().SetTimer(
			AutoFireTimer,
			this,
			&AShooterCharacter::AutoFireReset,AutomaticFireRate);
	}
}

void AShooterCharacter::AutoFireReset()
{
	bShouldFire = true;

	if (bFireButtonPressed)
	{
		StartFireTimer();
	}
}

bool AShooterCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector OutHitLocation)
{
	//��������� �������� ������� Viewport
	//get current size of the Viewport
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	//�������������� ����������� � �������� ������������
	//Get screen space location of crosshairs
	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	//��������� �������� ��������� � ����������� ��� �����������
	//Get world position and direction of crosshair
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		//����������� �� �������� ��������� ����������� �� ���
		//Trace from crosshair world location outward
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 50'000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);

		//�������� ����������� ��� ����������� AreaSphere
		// check line trace after overlapping AreaSphere
		//DrawDebugLine(GetWorld(),Start,End,FColor::Red,false, 2.f);

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

void AShooterCharacter::TraceForItems()
{
	if (bShouldTraceForItems)
	{

		FHitResult ItemTraceResult;
		FVector HitLocation;
		TraceUnderCrosshairs(ItemTraceResult, HitLocation);
		if (ItemTraceResult.bBlockingHit)
		{
			AItem* HitItem = Cast<AItem>(ItemTraceResult.Actor);
			if (HitItem && HitItem->GetPickupWidget())
			{
				//�������� Widget ������������ ��������
				//Show Item's pickup Widget
				HitItem->GetPickupWidget()->SetVisibility(true);
			}
		}
	}
}

bool AShooterCharacter::GetBeamEndLocation(
	const FVector& MuzzelSocketLocation, 
	FVector& OutBeamLocation)
{
	//�������� �� ��������� ����������� �� �����������
	//Check for crosshair trace hit
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);

	if (bCrosshairHit)
	{
		//��������� ��������� ������� �����
		//Tentative beam location
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else // no crosshair trace hit
	{
		//OutBeamLocation ��� �������� �������������� �������� �����������
		//OutBeamLocation is the END location for the line trace
	}

	
	//2 ����������� �� ������ ������
	//Perform a second trace. this time from the gun barrel
	FHitResult WeaponTraceHit;
	const FVector WeaponTraceStart{ MuzzelSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - MuzzelSocketLocation };
	const FVector WeaponTraceEnd{ MuzzelSocketLocation + StartToEnd * 1.25f };
	GetWorld()->LineTraceSingleByChannel(
		WeaponTraceHit,
		WeaponTraceStart,
		WeaponTraceEnd,
		ECollisionChannel::ECC_Visibility);

	//���� �� ������ ����� ������� � �������� ������ ������� �����
	//Object between barrel and 
	if (WeaponTraceHit.bBlockingHit)
	{
		OutBeamLocation = WeaponTraceHit.Location;
		return true;
	}

	return false;
}

//������������� �������� bAiming � ����������� �� ������� ������
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

	//��������� ���������������� �������� � ����������� �� ������������
	//Change look sensitivity based on aiming
	SetLookRates();
	
	//���������� ��������� ���������� �����������
	//Calculate crosshair spread multuplier
	CalculateCrosshairSpread(DeltaTime);

	//�������� OverlappedItemCount, ����� ����������� ��� ���������
	//Check OverlappedItemCount, then trace for items
	TraceForItems();
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
		&AShooterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this, 
		&AShooterCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this,
		&AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", IE_Released, this,
		&AShooterCharacter::AimingButtonReleased);
}

void AShooterCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if (OverlappedItemConut + Amount <= 0)
	{
		OverlappedItemConut = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemConut += Amount;
		bShouldTraceForItems = true;
	}
}

