// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTER_PROJECT_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:

#pragma region reg2
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Вызывается для ввода вперёд/назад
	//Called for forwards/backwards input
	void MoveForward(float Value);

	//Вызывается для ввода в бок
	//Called for side to side input
	void MoveRight(float Value);

	//вызывается посредством ввода для вращения с заданной скоростью
	//Called via input to turn at a given rate
	//@param Rate this is a normalized rate. 1 = 100% of disired turn rate
	void TurnAtRate(float Rate);

	//вызывается посредством ввода для поворота камеры вверх/вниз с заданной скоростью
	//Called via input to look up/down at a given rate
	//@param Rate this is a normalized rate. 1 = 100% of disired turn rate
	void LookUpAtRate(float Rate);

	//вращение контроллера на основе движения мыши по Х
	//Rotate controller based on mouse X movement
	//@param Value The inpun value from mouse movement
	void Turn(float Value);

	//вращение контроллера на основе движения мыши по Y
	//Rotate controller based on mouse Y movement
	//@param Value The inpun value from mouse movement
	void LookUp(float Value);
	
	//вызывается, когда нажата кнопка стрельбы
	//called when the Fire Button is pressed
	void FireWeapon();

	//получить координаты конца дымного следа
	//get end beam location
	bool GetBeamEndLocation(const FVector &MuzzelSocketLocation, FVector &OutBeamLocation);

	void AimingButtonPressed();
	void AimingButtonReleased();

	//плавное прицеливание
	//smooth aiming
	void CameraInterpZoom(float DeltaTime);

	//устанавливает базовую скоость вращения в зависимости от прицеливания
	//Set BaseTurnRate and BaseLookUpRate based on aiming
	void SetLookRates();

	void CalculateCrosshairSpread(float DeltaTime);

	UFUNCTION()
	void FinishCrosshairBulletFire();

	void StartCrosshairBulletFire();

	void FireButtonPressed();
	void FireButtonReleased();

	void StartFireTimer();

	UFUNCTION()
	void AutoFireReset();

	//Линейная трассировка для предметов под перекрестием
	//Line trace for items under the crosshairs
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector OutHitLocation);

	//Трассировка для предметов, в случае если OverlappedItemCount <0
	//Trace for items if OverlappedItemCount >0
	void TraceForItems();

	//Берёт оружие и прикрепляет его к мешу
	//Takes a weapon and attaches it to the mesh
	void EquipWeapon(AWeapon* WeaponToEquip);

	//Открепляет оружие и позволяет ему падать на землю
	//Detach weapon and let it fall to the ground
	void DropWeapon();

	void SelectButtonPressed();

	void SelectButtonReleased();

	//Сбрасывает текущее экипированное оружие и экипирует TraceHitItem
	//Drops currently equipped weapon and equips TraceHitItem
	void SwapWeapon(AWeapon* WeaponToSwap);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

#pragma region reg1
	//CameraBoom позиционирует камеру за персонажем 
	//CameraBoom positioning the camera behind the character
	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//Самера, следующая за игроком
	//Camera that follows the character
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


	//Базовое значение для поворота в градусах/сек
	//Base turn rate, in deg/sec.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	//Базовое значение для взгляда вверх/вниз в градусах/сек
	//Base look up/down rate, in deg/sec.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	//рандомный звук выстрела из оружия
	//Randomized gunshoot sound cue
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	//вспышка, призванная в сокете ствола
	//Flash spawned at BarrelSocket
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzelFlash;

	//монтаж для стрельбы из оружия
	//Montage for firing the weapon
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;

	//частицы, призываемые при попадании пули
	//Particles spawned upon bullet impact
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	//дымный след от пуль
	//Smoke trail for bullet
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;


	//true при прицеливании 
	//true when aiming
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	//стандартное значение поля обзора камеры
	//Default camera field of view value
	float CameraDefaultFOV;
	//значение поля обзора, когда сделано приблежение
	float CameraZoomedFOV;

	//текущее поле обзора в данном кадре
	//current field of view this frame
	float CameraCurrentFOV;

	//скорость интерполяции приблежения камеры при прицеливании
	//Interpolation speed for zooming aiming
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpspeed;

	//скорость вращения по вертикали не в прицеливании
	//Turn rate while not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;
	//скорость вращения по горизонталине в прицеливании
	//Look up rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;
	//скорость вращения по горизонтали в прицеливании
	//Turn rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;
	//скорость вращения по вертикали в прицеливании
	//Look up rate when aimng
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;

	//множители обзора при использовании мыши не в прицеливании
	//Scale factor for mouse look sensitivity. Turn rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0",UIMin = "0.0", UIMax = "1.0"))
	float MouseHipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;

	//множители обзора при использовании мыши  в прицеливании
	//Scale factor for mouse look sensitivity. LookUp rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;

	//определяет разброс перекрестия
	//defines the crosshair spread
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;
	//компонент скорости для разброса перекрестия
	//Velocity component for crosshairs spread
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairVelosityFactor;
	//компонент в воздехе для разброса перекрестия
	//in air component for crosshairs spread
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAimFactor;
	//компонент стрельбы для разброса перекрестия
	//Shooting component for crosshairs spread
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;


	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	//левая кнопка мыши
	//Left mouse button bressed
	bool bFireButtonPressed;

	//имеет значение true - когда может стрелять,false - когда ожидает возможности стрелять
	//True when we can fire. False when waiting for the timer
	bool bShouldFire;

	//темп автоматического огня
	//rate of automatic gun fire
	float AutomaticFireRate;

	//устанавливает таймер между выстрелами
	//sets a timer between gunshots
	FTimerHandle AutoFireTimer;

	//Имеет значение true если нужно делать трассировку каждый кадр для обнаружения предметов
	//True if we should trace every fame for items
	bool bShouldTraceForItems;
#pragma endregion reg1

	//Кол-во пересечённых предметов
	//Number of overlapped AItems
	int8 OverlappedItemConut;

public:

	//Возвращает субобъект CameraBoom
	//Returns CameraBoom subobject
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//возвращает FollowCamera
	//Returns FollowCamera subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	//возвращает bAiming
	//Returns bAiming
	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const { return CrosshairSpreadMultiplier; }

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemConut; }

	//Увеличивает или уменьшает OverlappedItemConut и обновляет bShouldTraceForItems
	//Adds/substracts to/from OverlappedItemConut and updates bShouldTraceForItems
	void IncrementOverlappedItemCount(int8 Amount);
};
