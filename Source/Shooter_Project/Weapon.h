// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Engine/DataTable.h"
#include "WeaponType.h"
#include "Weapon.generated.h"


USTRUCT(BlueprintType)
struct FWeaponDataTable: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapasity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Materialindex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClipBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneToHide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic;
};

UCLASS()
class SHOOTER_PROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	
	void StopFalling();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	void FinishMovingSlide();

	void UpdateSlideDisplacement();

private:

#pragma region reg1
	FTimerHandle ThrowWeaponTimer;

	float ThrowWeaponTime;

	bool bFalling;

	//Кол-во патрон для этого оружия
	//Ammo count for this Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	//Тип оружия
	//The type of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	//Тип патронов для этого оружия
	//The type of ammo for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	//Максимум патронов помещающихся в оружие
	//Maximum ammo that our weapon can hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	//FName для секции монтажа перезарядки
	//FName for the reload montage section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	//True когда магазин перемещается во время перезарядки
	//True when moving the clip while reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bMoovingClip;

	//Название для кости магазина
	//Name for the clip bone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName;

	//Таблица данных для свойств оружия
	//Data tablefor weapon properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponDataTable;

	int32 PreviousMaterialIndex;

	//Текстуры для перекрестий оружия
	//Textures for the weapon crosshairs
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsBottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UTexture2D* CrosshairsTop;

	//Скорость с которой стреляет оружие в автоматическом режиме
	//The speed which automatic fire happens
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	float AutoFireRate;

	//Частицы, которые призываются в местоположение сокета стола
	//Particle system spawnrd at the BarrelSocket
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	//Звук, который воспроизводится во время стрельбы оружия
	//Sound played when the weapon is fired
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	USoundCue* FireSound;
#pragma endregion reg1
	//Имя кости, которая будет спрятана
	//Name of the bone to hide
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	FName BoneToHide;

	//Значение того,на сколько затвор пистолета отодвигается назад, во время выстрела
	//Amount that the slide is pushed back during pistol
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float SlideDisplacement;

	//Кривая для смещения затвора
	//Curve for the slide displacement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SlideDisplacementCurve;

	//Таймер для обновления смещения затвора
	//Timer handle for updating SlideDisplacement
	FTimerHandle SlideTimer;

	//Время для смещения затвора во время выстрела из пистолета
	//Time for displacement the slide during pistol fire
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float SlideDisplacementTime;

	//Истина при перемещении затвора пистолета
	//True when moving the pistol slide
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	bool bMovingSlide;

	//Максимальное расстояние для движения затвора пистолета
	//Max distance for the slide on the pistol
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pistol, meta = (AllowPrivateAccess = "true"))
	float MaxSlideDisplacement;

	//Истина для автоматической стрельбы
	//True for auto gunfire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bAutomatic;

public:

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE void GetReloadMontageSection(FName Name) {  ReloadMontageSection = Name; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetClipBoneName(FName Name) {  ClipBoneName = Name; }
	FORCEINLINE void SetMovingClip(bool Move) { bMoovingClip = Move; }
	FORCEINLINE float GetAutoFireRate() const { return AutoFireRate; }
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE bool GetAutomatic() const { return bAutomatic; }


	void StartSlideTimer();


	void ReloadAmmo(int32 Amount);

	//Добавляет импульс к оружию
	//Add impulse to the weapon
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	//Вызывается из класса персонажа при стрельбе из оружия
	//Called from Character class when firing weapon
	void DecrementAmmo();

	//true - если магазин полон, иначе false
	//return true if magazine full, else - false
	bool ClipIsFull();
};
