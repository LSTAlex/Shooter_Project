#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AmmoType.h"
#include "ShooterCharacter.generated.h"



UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FInterpLocation
{
	GENERATED_BODY()

	//Компанент сцены для использования местоположения во время интерполяции
	//Scene component to use its location for interping
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	//Количество предметов, совершающих интерполяцию в это местоположение компонента сцены
	//Number of items interping to/at this scene comp location
	int32 ItemCount;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHightlightIconDelegate, int32, SlotIndex, bool, BStartAnimation);


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
	bool GetBeamEndLocation(const FVector& MuzzelSocketLocation, FVector& OutBeamLocation);

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

	//призывает стандартное оружие и экипировывает его
	//Spawn a default weapon and equip it
	class AWeapon* SpawnDefaultWeapon();

	//Берёт оружие и прикрепляет его к мешу
	//Takes a weapon and attaches it to the mesh
	void EquipWeapon(AWeapon* WeaponToEquip, bool bSwapping = false);

	//Открепляет оружие и позволяет ему падать на землю
	//Detach weapon and let it fall to the ground
	void DropWeapon();

	void SelectButtonPressed();

	void SelectButtonReleased();

	//Сбрасывает текущее экипированное оружие и экипирует TraceHitItem
	//Drops currently equipped weapon and equips TraceHitItem
	void SwapWeapon(AWeapon* WeaponToSwap);

	//Инифиализация карты патронов значениями патронов
	//Initialize the Ammo Map with ammo values
	void InitializeAmmoMap();

	//Проверяет наличие потронов в оружие
	//Check to make sure our weapon has ammo
	bool WeaponHasAmmo();

	//FireWeapon функции
	//FireWeapon functions
	void PlayFireSound();
	void SendBullet();
	void PlayGunFireMontage();

	//Привязка к кнопке R
	//Bound to the R key
	void ReloadButtonPressed();

	//Управляет перезарядкой оружия
	//Handle reloading of the weapon
	void ReloadWeapon();

	//Проверяет есть ли патроны того типа, которые использует оружие
	//Checks to see of we have ammo of the equipped weapon ammo type
	bool CarryingAmmo();

	//Вызывается из анимационного Blueprint с помощью оповещения GrabClip
	//Called from animation Blueprint with GrabClip notyfy
	UFUNCTION(BlueprintCallable)
	void GrabClip();

	//Вызывается из анимационного Blueprint с помощью оповещения ReleaseClip
	//Called from animation Blueprint with ReleaseClip notyfy
	UFUNCTION(BlueprintCallable)
	void ReleaseClip();

	void CrouchButtonPressed();

	void CrouchButtonReleased();

	//Interps capsule half height when character crouching/standing
	//Интерполирует половину высоты капсулы когда персонаж вприсяде/стоит
	void InterpCapsuleHalfHaight(float DelataTime);
#pragma endregion reg2
	void Aim();
	void StopAiming();

	void PickupAmmo(class AAmmo* Ammo);

	void InitializeInterpLocations();

	void FKeyPressed();
	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKeyPressed();
	void FourKeyPressed();
	void FiveKeyPressed();

	void ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemindex);

	int32 GetEmptyinventorySlot();

	void HightlightInventorySlot();
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

#pragma region reg1
	//CameraBoom позиционирует камеру за персонажем 
	//CameraBoom positioning the camera behind the character
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
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
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CameraDefaultFOV;
	//значение поля обзора, когда сделано приблежение
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
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

	//Кол-во пересечённых предметов
	//Number of overlapped AItems
	int8 OverlappedItemConut;

	//AItem в который попала трассировка в последнем кадре 
	//AItem we hit last frame
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItemLastFrame;

	//Текущее экипированное оружие
	//Currently qequipped Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;

	//Установить это в Blueprints для стандартного класса оружия
	//Set this in Blueprints for the default Weapon class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	//Предмет в который попал луч трассировки от TraceForItems(может быть null)
	//The item currently hit by our trace in TraceForItems(could be null)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	//Расстояние вперёд от камеры до точки интерполяции
	//Distance outward from the camera for the interp destination
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float CameraInterpDistance;

	//Расстояние вверх от камеры до точки назрачения
	//Distance upward from the camera for the interp destination
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float CameraInterpElevation;

	//Карта для отслеживания патронов разных типов
	//Map to keep track of ammo of the different ammo types
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;

	//Стартовое кол-во патронов 9мм
	//Starting amount 9mm ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo;

	//Стартовое кол-во патронов для винтовки
	//Starting amount Assault Rifle ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo;

	//Боевое состояние, можно стрелять или перезаться только в состоянии Unoccupied
	//Combat state. can only fire or reload if Unoccupied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	//Монтаж для анимации перезарядки
	//Montage for reload animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	//Трансформация магазина когда мы берём магазин при перезарядке
	//Transform of the clip when we first grab the clip during reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;

	//Компонент сцены для прикрепления к руке персонажа во время перезарядки
	//Scene component to attach to the character hand during reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent;

	//True when crouching
	//Истенен во время приседания
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bCrouching;


	//Regular movement speed
	//Обычная скорость ходьбы
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;

	//Crouch movement speed
	//Скорость ходьбы в присядя
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchMovementSpeed;

	//Current half height of the capsule
	//Текущее половина высоты капсулы
	float CurrentCapsuleHalfHeight;

	//Half height of the capsule when not crouching
	//Половина высоты капсулы, когда не вприсяде 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float StandingCapsuleHalfHeight;

	//Half height of the capsule when crouching
	//Половина высоты капсулы когда вприсяде
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CrouchingCapsuleHalfHeight;

	//Используется что бы знать нажата ли кнопка прицеливания
	bool bAimingButtonPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	USceneComponent* InterpComp6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponInterpComp;

	//массив структур местоположений интерполяций
	//Array of interp location structs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FInterpLocation> InterpLocations;

	FTimerHandle PickupSoundTimer;
	FTimerHandle EquipSoundTimer;

	bool bShouldPlayPickupSound;
	bool bShouldPlayEquipSound;

	void ResetPickupSound();
	void ResetEquipSound();

	//Время,которое должно пройти до воспроизведения повторного звука поднятия
	//Time to wait before we can play another Pickup Sound
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = Items, meta = (AllowPrivateAccess = "true"))
	float PickupSoundResetTime;
#pragma endregion reg1
	//Время,которое должно пройти до воспроизведения повторного звука экипировывания
	//Time to wait before we can play another Equip Sound
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float EquipSoundResetTime;
	
	//Массив элементов типа AItem для инвенторя
	//An array of AItem for inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<AItem*> Inventory;

	const int32 INVENTORY_CAPACITY{ 6 };

	//Делегат для пересылки информации слота к панели инвентаря при экипировывании предмета
	//Delegate for sending slot information to Inventorybar when equipping
	UPROPERTY(BlueprintAssignable, Category = Delegates, meta = (AllowPrivateAccess = "true"))
	FEquipItemDelegate EquipItemDelegate;

	//Делегат для пересылки информации слота о воспроизведении анимации
	//Delegate for sending slot information for playing the icon animation
	UPROPERTY(BlueprintAssignable, Category = Delegates, meta = (AllowPrivateAccess = "true"))
	FHightlightIconDelegate HightlightIconDelegate;

	//Индекс текущего выделенного слота
	//The index for the currently Hightlighted slot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int32 HightlightSlot;

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
	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemConut; }
	//возвращает состояние персонажа
	//Return CombatState character
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE bool GetCrouching() const { return bCrouching; }
	FORCEINLINE bool ShouldPlayPickupSound() const { return bShouldPlayPickupSound; }
	FORCEINLINE bool ShouldPlayEquipSound() const { return bShouldPlayEquipSound; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const { return CrosshairSpreadMultiplier; }

	//Увеличивает или уменьшает OverlappedItemConut и обновляет bShouldTraceForItems
	//Adds/substracts to/from OverlappedItemConut and updates bShouldTraceForItems
	void IncrementOverlappedItemCount(int8 Amount);

	void GetPickupItem(AItem* Item);

	FInterpLocation GetInterpLocations(int32 Index);

	//Возвращает индекс элемента массива положения интерполяции с наименьшим кол-вом предметов
	//Return the index in InterpLocations array whith the lowest ItemCount
	int32 GetInterpLocationIndex();

	void IncrementInterpLocItemCount(int32 Index, int32 Amount);

	void StartPickupSoundTimer();
	void StartEquipSoundTimer();

	void UnHightlightInventorySlot();
};
