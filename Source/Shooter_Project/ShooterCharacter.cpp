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
#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Ammo.h"


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
	ZoomInterpspeed(20.f),
	//факторы расширения перекрестия
	//Crosshair spread factors
	CrosshairSpreadMultiplier(0.f),
	CrosshairVelosityFactor(0.f),
	CrosshairInAirFactor(0.f),
	CrosshairInAimFactor(0.f),
	CrosshairShootingFactor(0.f),
	//переменные таймера стрельбы
	//bullet fire timer variables
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	bShouldFire(true),
	bFireButtonPressed(false),
	//Переменные трассировки предметов
	//Item trace variable
	bShouldTraceForItems(false),
	OverlappedItemConut(0),
	//Переменные определяющие местоположение интерполяции перед самерой
	//Camera interp location variables
	CameraInterpDistance(250.f),
	CameraInterpElevation(65.f),
	//Стартовое кол-во патронов
	//Starting ammo amount
	Starting9mmAmmo(120),
	StartingARAmmo(60),
	//Боевые состояния
	//Combat variables
	CombatState(ECombatState::ECS_Unoccupied),
	bCrouching(false),
	BaseMovementSpeed(650.f),
	CrouchMovementSpeed(300.f),
	StandingCapsuleHalfHeight(88.f),
	CrouchingCapsuleHalfHeight(44.f),
	bAimingButtonPressed(false),
	//Свойство таймера поднятий
	//Pickup sound timer properties
	bShouldPlayPickupSound(true),
	bShouldPlayEquipSound(true),
	PickupSoundResetTime(0.2f),
	EquipSoundResetTime(0.2f),
	//Свойства анимации иконки
	//Icon animation property
	HightlightSlot(-1)
	
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Создаёт CameraBoom (притягивается к персонажу, если есть коллизии)
	//Create CameraBoom (pulls in towards the character if the is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//Следует за персонажем на этом расстоянии
	//The camera foolows at this distance behind the character
	CameraBoom->TargetArmLength = 239.f;
	//вращает рычаг, основываясь на вращении контроллера
	//Rotate the arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.f, 35.f, 80.f);

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

	//Создаёт компонент сцены руки
	//Create hand scene component
	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComp"));

	//Создание компонентов интерполяции
	//Create interpolation components
	WeaponInterpComp = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Interpolation Component"));
	WeaponInterpComp->SetupAttachment(GetFollowCamera());

	InterpComp1 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 1"));
	InterpComp1->SetupAttachment(GetFollowCamera());

	InterpComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 2"));
	InterpComp2->SetupAttachment(GetFollowCamera());

	InterpComp3 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 3"));
	InterpComp3->SetupAttachment(GetFollowCamera());

	InterpComp4 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 4"));
	InterpComp4->SetupAttachment(GetFollowCamera());

	InterpComp5 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 5"));
	InterpComp5->SetupAttachment(GetFollowCamera());

	InterpComp6 = CreateDefaultSubobject<USceneComponent>(TEXT("Interpolation Component 6"));
	InterpComp6->SetupAttachment(GetFollowCamera());

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

	//Призыв стандартного оружия и  его прикрепление к мешу
	//Spawn the default weapon and attach it to the mesh
	EquipWeapon(SpawnDefaultWeapon());
	Inventory.Add(EquippedWeapon);
	EquippedWeapon->SetISlotIndex(0);
	EquippedWeapon->DisableCustomDepth();
	EquippedWeapon->DisableGlowMaterial();

	InitializeAmmoMap();

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	//Создание структур положений интерполяции. Добавление в массив
	//Create FInterpLocation structs for each interp location.Add array
	InitializeInterpLocations();
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

	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (EquippedWeapon == nullptr)return;

	if (WeaponHasAmmo())
	{
		StartFireTimer();

		//Воспроизведение звуков выстрела
		//Player fire sound
		PlayFireSound();

		//Посылает пулю
		//Send bullet
		SendBullet();

		//Воспроизведение HipFireMontage
		//Play HipFireMontage
		PlayGunFireMontage();

		//Вычитает 1 из кол-ва патронов в оружии
		//Subtract 1 from the weapon ammo
		EquippedWeapon->DecrementAmmo();
		
		//запускает таймер BulletFire для перекрестий
		//Start BulletFire timer for crosshair
		StartCrosshairBulletFire();
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

	//прицеливаемся ли мы?
	//are we aiming?
	if (bAiming)
	{
		//небольшое сжатие перекрестия очень быстро
		//Shrink crosshair a small very quickly
		CrosshairInAimFactor = FMath::FInterpTo(
			CrosshairInAimFactor, 
			0.6f,
			DeltaTime, 
			30.0f);
	}
	else
	{
		//очень быстро возвращает пперекрестие в нормальное состояние
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

	FireWeapon();
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AShooterCharacter::StartFireTimer()
{
	if (EquippedWeapon == nullptr)return;
	
	CombatState = ECombatState::ECS_FireTimerInProgress;

	GetWorldTimerManager().SetTimer(
		AutoFireTimer,
		this,
		&AShooterCharacter::AutoFireReset,EquippedWeapon->GetAutoFireRate());
}

void AShooterCharacter::AutoFireReset()
{
	CombatState = ECombatState::ECS_Unoccupied;
	if (WeaponHasAmmo())
	{
		if (bFireButtonPressed)
		{
			FireWeapon();
		}
	}
	else
	{
		//Перезарядка оружия
		//Reload weapon
		ReloadWeapon();
	}
}

bool AShooterCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector OutHitLocation)
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
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	//Получение мирового положения и направления для перекрестия
	//Get world position and direction of crosshair
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		//трассировка от мирового положения перекрестия во вне
		//Trace from crosshair world location outward
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 50'000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);

		//проверка трассировки при пересечении AreaSphere
		//check line trace after overlapping AreaSphere
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
			TraceHitItem = Cast<AItem>(ItemTraceResult.Actor);
			const auto TraceHitWeapon = Cast<AWeapon>(TraceHitItem);
			if (TraceHitWeapon)
			{
				if (HightlightSlot == -1) 
				{
					//Сейчас слот не выделяется. Выделить один 
					//Not currently highlighting slot.highlight one
					HightlightInventorySlot();
				}
			}
			else
			{
				//Был ли слот выделен? 
				//Is a slot being highlight?
				if (HightlightSlot != -1)
				{
					//Снять выделение с слота
					//UnHighlight the slot
					UnHightlightInventorySlot();
				}
				
			}

			if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterping)
			{
				TraceHitItem = nullptr;
			}

			if (TraceHitItem && TraceHitItem->GetPickupWidget())
			{
				//показать Widget поднимаемого предмета
				//Show Item's pickup Widget
				TraceHitItem->GetPickupWidget()->SetVisibility(true);
				TraceHitItem->EnableCustomDepth();
				if (Inventory.Num() >= INVENTORY_CAPACITY)
				{
					//Инвентарь полон
					//Inventory is full
					TraceHitItem->SetCharacterInventoryFull(true);
				}
				else
				{
					//В инвентаре есть место
					//Inventory has room
					TraceHitItem->SetCharacterInventoryFull(false);
				}
			}

			if (TraceHitItemLastFrame)
			{
				if (TraceHitItem != TraceHitItemLastFrame)
				{
					//трассировка попала в другой AItem в этом кадре или AItem нет 
					//We are hitting a different AItem this frame or AItem is null
					TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
					TraceHitItemLastFrame->DisableCustomDepth();
				}
			}
			//хранится ссылка на предмет найденный при помощи трассировки для следующего кадра 
			//storage a reference to HitItem for next frame
			TraceHitItemLastFrame = TraceHitItem;
		}
	}
	else if (TraceHitItemLastFrame)
	{
		//Никакие предметы больше не пересечены, не нужно показывать PickupWidget предметов
		//No longer overlapping any items. Item last frame should not show PickupWidget
		TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
		TraceHitItemLastFrame->DisableCustomDepth();
	}
}

AWeapon* AShooterCharacter::SpawnDefaultWeapon()
{
	//Проверка переменной TSubclassOf
	//Check the TSubclassOf variable
	if (DefaultWeaponClass)
	{
		//Призыв оружия
		//Spawn the Weapon
		return GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
	}
	return nullptr;
}

void AShooterCharacter::EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping)
{
	if (WeaponToEquip)
	{	
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (HandSocket)
		{
			//Прикрепление оружия к сокету руки RightHandSocket
			//Attach the weapon to the hand socket RightHandSocket
			HandSocket->AttachActor(WeaponToEquip, GetMesh());
		}

		if (EquippedWeapon == nullptr)
		{
			//-1 == ещё нет экипированного оружия. Не нужно делать обратную анимацию иконки
			EquipItemDelegate.Broadcast(-1, WeaponToEquip->GetISlotIndex());
		}
		else if (!bSwapping)
		{
			EquipItemDelegate.Broadcast(EquippedWeapon->GetISlotIndex(), WeaponToEquip->GetISlotIndex());
		}

		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
	}
}

void AShooterCharacter::DropWeapon()
{
	if (EquippedWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);
		EquippedWeapon->SetItemState(EItemState::EIS_Falling);
		EquippedWeapon->ThrowWeapon();
	}
}

void AShooterCharacter::SelectButtonPressed()
{
	if (CombatState != ECombatState::ECS_Unoccupied)return;
	if (TraceHitItem)
	{
		TraceHitItem->StartItemCurve(this);
		TraceHitItem = nullptr;
	}
}

void AShooterCharacter::SelectButtonReleased()
{
}

void AShooterCharacter::SwapWeapon(AWeapon* WeaponToSwap)
{
	if (Inventory.Num() -1 >= EquippedWeapon->GetISlotIndex())
	{
		Inventory[EquippedWeapon->GetISlotIndex()] = WeaponToSwap;
		WeaponToSwap->SetISlotIndex(EquippedWeapon->GetISlotIndex());
	}

	DropWeapon();
	EquipWeapon(WeaponToSwap, true);
	TraceHitItem = nullptr;
	TraceHitItemLastFrame = nullptr;
}

void AShooterCharacter::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::EAT_9mm, Starting9mmAmmo);
	AmmoMap.Add(EAmmoType::EAT_AR, StartingARAmmo);
}

bool AShooterCharacter::WeaponHasAmmo()
{
	if (EquippedWeapon == nullptr)return false;

	return EquippedWeapon->GetAmmo() > 0;
}

void AShooterCharacter::PlayFireSound()
{
	if (EquippedWeapon->GetFireSound())
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->GetFireSound());
	}
}

void AShooterCharacter::SendBullet()
{
	const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemMesh());

		if (EquippedWeapon->GetMuzzleFlash())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeapon->GetMuzzleFlash(), SocketTransform);
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
}

void AShooterCharacter::PlayGunFireMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
}

void AShooterCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
}

void AShooterCharacter::ReloadWeapon()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (EquippedWeapon == nullptr) return;
		
	//Есть ли патроны правильного типа
	//Do we have ammo of the correct type 
	if (CarryingAmmo() && !EquippedWeapon->ClipIsFull())
	{
		if (bAiming)
		{
			StopAiming();
		}
		CombatState = ECombatState::ECS_Reloading;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(EquippedWeapon->GetReloadMontageSection());
		}
	}		
}

bool AShooterCharacter::CarryingAmmo()
{
	if (EquippedWeapon == nullptr) return false;
	
	auto AmmoType = EquippedWeapon->GetAmmoType();

	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}

	return false;
}

void AShooterCharacter::GrabClip()
{
	if (EquippedWeapon == nullptr) return;
	if (HandSceneComponent == nullptr) return;
	
	//Индекс кости магазина экипированного оружия
	//Index for the clip bone on the EquippedWeapon
	int32 ClipBoneIndex{ EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName())};
	//Хранится значение трансформации магазина
	//Store the transform of the clip
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Hand_L")));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);
}

bool AShooterCharacter::GetBeamEndLocation(
	const FVector& MuzzelSocketLocation, 
	FVector& OutBeamLocation)
{
	//проверка на попадание трассировки от перекрестия
	//Check for crosshair trace hit
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);

	if (bCrosshairHit)
	{
		//Временное положение дысного следа
		//Tentative beam location
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else // no crosshair trace hit
	{
		//OutBeamLocation это конечное местоположение линейной трассировки
		//OutBeamLocation is the END location for the line trace
	}

	//2 трассировка от ствола оружия
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

	//есть ли объект между стволом и конечной точкой дымного следа
	//Object between barrel and 
	if (WeaponTraceHit.bBlockingHit)
	{
		OutBeamLocation = WeaponTraceHit.Location;
		return true;
	}

	return false;
}

//устанавливает значение bAiming в зависимости от нажатия кнопки
//Set bAiming to true or false with button
void AShooterCharacter::AimingButtonPressed()
{
	bAimingButtonPressed = true;
	if (CombatState != ECombatState::ECS_Reloading)
	{
		Aim();
	}
}

void AShooterCharacter::AimingButtonReleased()
{
	bAimingButtonPressed = false;
	StopAiming();
}

void AShooterCharacter::ReleaseClip()
{
	EquippedWeapon->SetMovingClip(false);
}

void AShooterCharacter::CrouchButtonPressed()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		bCrouching = !bCrouching;
	}
	if (bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
}

void AShooterCharacter::CrouchButtonReleased()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		bCrouching = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
}

void AShooterCharacter::InterpCapsuleHalfHaight(float DelataTime)
{
	float TargetCapsuleHalfHeight{};

	if (bCrouching)
	{
		TargetCapsuleHalfHeight = CrouchingCapsuleHalfHeight;
	}
	else
	{
		TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	}

	const float InterpHalfHeight{ FMath::FInterpTo(
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
		TargetCapsuleHalfHeight,
		DelataTime,
		20.f)};

	//Nagative value if crouching; Positive value if standing;
	//При приседании отрицательное значение. Положительное значение когда персонаж стоит.
	const float DeltaCapsuleHalfHeight{ InterpHalfHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
	const FVector MeshOffset{ 0.f,0.f,-DeltaCapsuleHalfHeight };
	GetMesh()->AddLocalOffset(MeshOffset);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

void AShooterCharacter::Aim()
{
	bAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
}

void AShooterCharacter::StopAiming()
{
	bAiming = false;
	if (!bCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}
}

void AShooterCharacter::PickupAmmo(AAmmo* Ammo)
{
	//Проверяется содержится ли в AmmoMap тип патронов патронов
	//Check to see if AmmoMap contains Ammo's AmmoType
	if (AmmoMap.Find(Ammo->GetAmmoType()))
	{
		//Получает кол-во патронов этого типа в карте патронов
		//Get amount of ammo in our AmmoMap for Ammo's type
		int32 AmmoCount{ AmmoMap[Ammo->GetAmmoType()] };
		AmmoCount += Ammo->GetItemCount();
		//Устанавливает кол-во патронов этого типа в карте патронов
		//Set the amount of ammo in the Map for this type
		AmmoMap[Ammo->GetAmmoType()] = AmmoCount;
	}
	if (EquippedWeapon->GetAmmoType() == Ammo->GetAmmoType())
	{
		//Проверяет пусто ли оружие
		//Check to see if the gun is empty
		if (EquippedWeapon->GetAmmo() == 0)
		{
			ReloadWeapon();
		}
	}

	Ammo->Destroy();
}

void AShooterCharacter::InitializeInterpLocations()
{
	FInterpLocation WeaponLocation{ WeaponInterpComp, 0 };
	InterpLocations.Add(WeaponLocation);

	FInterpLocation Interploc1{ InterpComp1, 0 };
	InterpLocations.Add(Interploc1);

	FInterpLocation Interploc2{ InterpComp2, 0 };
	InterpLocations.Add(Interploc2);

	FInterpLocation Interploc3{ InterpComp3, 0 };
	InterpLocations.Add(Interploc3);

	FInterpLocation Interploc4{ InterpComp4, 0 };
	InterpLocations.Add(Interploc4);

	FInterpLocation Interploc5{ InterpComp5, 0 };
	InterpLocations.Add(Interploc5);

	FInterpLocation Interploc6{ InterpComp6, 0 };
	InterpLocations.Add(Interploc6);
}

void AShooterCharacter::FKeyPressed()
{
	if (EquippedWeapon->GetISlotIndex() == 0)return;
	
	ExchangeInventoryItems(EquippedWeapon->GetISlotIndex(), 0);
}

void AShooterCharacter::OneKeyPressed()
{
	if (EquippedWeapon->GetISlotIndex() == 1)return;

	ExchangeInventoryItems(EquippedWeapon->GetISlotIndex(), 1);
}

void AShooterCharacter::TwoKeyPressed()
{
	if (EquippedWeapon->GetISlotIndex() == 2)return;

	ExchangeInventoryItems(EquippedWeapon->GetISlotIndex(), 2);
}

void AShooterCharacter::ThreeKeyPressed()
{
	if (EquippedWeapon->GetISlotIndex() == 3)return;

	ExchangeInventoryItems(EquippedWeapon->GetISlotIndex(), 3);
}

void AShooterCharacter::FourKeyPressed()
{
	if (EquippedWeapon->GetISlotIndex() == 4)return;

	ExchangeInventoryItems(EquippedWeapon->GetISlotIndex(), 4);
}

void AShooterCharacter::FiveKeyPressed()
{
	if (EquippedWeapon->GetISlotIndex() == 5)return;

	ExchangeInventoryItems(EquippedWeapon->GetISlotIndex(), 5);
}

void AShooterCharacter::ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemindex)
{
	if ((CurrentItemIndex == NewItemindex) || (NewItemindex >= Inventory.Num()) || (CombatState!=ECombatState::ECS_Unoccupied))return;
	auto OldEquippedWeapon = EquippedWeapon;
	auto NewWeapon = Cast<AWeapon>(Inventory[NewItemindex]);
	EquipWeapon(NewWeapon);
	OldEquippedWeapon->SetItemState(EItemState::EIS_PickedUp);
	NewWeapon->SetItemState(EItemState::EIS_Equipped);
}

int32 AShooterCharacter::GetEmptyinventorySlot()
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i] == nullptr)
		{
			return i;
		}
	}

	if (Inventory.Num() < INVENTORY_CAPACITY)
	{
		return Inventory.Num();
	}

	//инвентарь полон
	//Inventory is full
	return -1;
}

void AShooterCharacter::HightlightInventorySlot()
{
	const int32 EmprySlot{ GetEmptyinventorySlot() };
	HightlightIconDelegate.Broadcast(EmprySlot, true);
	HightlightSlot = EmprySlot;
}

void AShooterCharacter::UnHightlightInventorySlot()
{
	HightlightIconDelegate.Broadcast(HightlightSlot, false);
	HightlightSlot = -1;
}

int32 AShooterCharacter::GetInterpLocationIndex()
{
	int32 LowestIndex = 1;
	int32 LowestCount = INT_MAX;

	for (int32 i = 1; i < InterpLocations.Num(); i++)
	{
		if (InterpLocations[i].ItemCount < LowestCount) 
		{
			LowestIndex = i;
			LowestCount = InterpLocations[i].ItemCount;
		}
	}

	return LowestIndex;
}

void AShooterCharacter::IncrementInterpLocItemCount(int32 Index, int32 Amount)
{
	if (Amount < -1 || Amount > 1)return;
	if (InterpLocations.Num() >= Index) 
	{
		InterpLocations[Index].ItemCount += Amount;
	}
}

void AShooterCharacter::StartPickupSoundTimer()
{
	bShouldPlayPickupSound = false;
	GetWorldTimerManager().SetTimer(PickupSoundTimer, this, &AShooterCharacter::ResetPickupSound, PickupSoundResetTime);
}

void AShooterCharacter::StartEquipSoundTimer()
{
	bShouldPlayEquipSound = false;
	GetWorldTimerManager().SetTimer(EquipSoundTimer, this, &AShooterCharacter::ResetEquipSound, EquipSoundResetTime);
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

	//проверка OverlappedItemCount, затем трассировка для предметов
	//Check OverlappedItemCount, then trace for items
	TraceForItems();

	//Interpolate the capsule half height based on crouching/standing
	//Интерполирует половину высоты капсулы в зависимости от вприсяде/стоит персонаж
	InterpCapsuleHalfHaight(DeltaTime);
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
	PlayerInputComponent->BindAction("Select", IE_Pressed, this,
		&AShooterCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction("Select", IE_Released, this,
		&AShooterCharacter::SelectButtonReleased);
	PlayerInputComponent->BindAction("ReloadButton", IE_Pressed, this,
		&AShooterCharacter::ReloadButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this,
		&AShooterCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this,
		&AShooterCharacter::CrouchButtonReleased);
	PlayerInputComponent->BindAction("FKey", IE_Pressed, this,
		&AShooterCharacter::FKeyPressed);
	PlayerInputComponent->BindAction("1Key", IE_Pressed, this,
		&AShooterCharacter::OneKeyPressed);
	PlayerInputComponent->BindAction("2Key", IE_Pressed, this,
		&AShooterCharacter::TwoKeyPressed);
	PlayerInputComponent->BindAction("3Key", IE_Pressed, this,
		&AShooterCharacter::ThreeKeyPressed);
	PlayerInputComponent->BindAction("4Key", IE_Pressed, this,
		&AShooterCharacter::FourKeyPressed);
	PlayerInputComponent->BindAction("5Key", IE_Pressed, this,
		&AShooterCharacter::FiveKeyPressed);
}

void AShooterCharacter::FinishReloading()
{
	//Обновление боевого состояния
	//Update the combat state
	CombatState = ECombatState::ECS_Unoccupied;

	if (bAimingButtonPressed)
	{
		Aim();
	}

	if (EquippedWeapon == nullptr) return;

	const auto AmmoType { EquippedWeapon->GetAmmoType() };

	//Обновление карты патронов
	//Update the AmmoMap
	if (AmmoMap.Contains(AmmoType))
	{
		//Кол-во патронов того же типа что и экипированное оружие, которое есть у персонажа
		//Amount of ammo the Character is carrying fo the equipped weapon
		int32 CarriedAmmo = AmmoMap[AmmoType];

		//Сколько места осталось в магазине в экипированном оружии
		//Space left in the magazine of EquippedWeapon
		const int32 MagEmptySpase = EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo();

		if (MagEmptySpase > CarriedAmmo)
		{
			//перезарядить магазин всеми патронами, которые есть
			//Reload the magazine with all the ammo we are carrying
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
		else
		{
			//заполнить магазин
			//fill the magazine
			EquippedWeapon->ReloadAmmo(MagEmptySpase);
			CarriedAmmo -= MagEmptySpase;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
	}
}

void AShooterCharacter::ResetPickupSound()
{
	bShouldPlayPickupSound = true;
}

void AShooterCharacter::ResetEquipSound()
{
	bShouldPlayEquipSound = true;
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

void AShooterCharacter::GetPickupItem(AItem* Item)
{
	Item->PlayEquipSound();

	auto Weapon = Cast<AWeapon>(Item);

	if (Weapon)
	{
		if (Inventory.Num() < INVENTORY_CAPACITY)
		{
			Weapon->SetISlotIndex(Inventory.Num());
			Inventory.Add(Weapon);
			Weapon->SetItemState(EItemState::EIS_PickedUp);
		}
		//Инвентарь полон. Заменяем оружие.
		//Inventory is full. Swap weapon
		else
		{
			SwapWeapon(Weapon);
		}
		
	}

	auto Ammo = Cast<AAmmo>(Item);
	if (Ammo)
	{
		PickupAmmo(Ammo);
	}
}

FInterpLocation AShooterCharacter::GetInterpLocations(int32 Index)
{
	if (Index <= InterpLocations.Num())
	{
		return InterpLocations[Index];
	}

	return FInterpLocation();
	
}