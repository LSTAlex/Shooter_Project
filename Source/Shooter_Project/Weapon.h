// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Engine/DataTable.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	EWT_SubmachinGun UMETA(DisplayName = "SubmachinGun"),
	EWT_AssaultRifle UMETA(DisplayName = "AssaultRifle"),
	EWT_MAX UMETA(DisplayName = "DefaultMAX")
};

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

private:

	FTimerHandle ThrowWeaponTimer;

	float ThrowWeaponTime;

	bool bFalling;

	//���-�� ������ ��� ����� ������
	//Ammo count for this Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	//��� ������
	//The type of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	//��� �������� ��� ����� ������
	//The type of ammo for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	//�������� �������� ������������ � ������
	//Maximum ammo that our weapon can hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	//FName ��� ������ ������� �����������
	//FName for the reload montage section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	//True ����� ������� ������������ �� ����� �����������
	//True when moving the clip while reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	bool bMoovingClip;

	//�������� ��� ����� ��������
	//Name for the clip bone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName;

	//������� ������ ��� ������� ������
	//Data tablefor weapon properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponDataTable;

public:

	//��������� ������� � ������
	//Add impulse to the weapon
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	//���������� �� ������ ��������� ��� �������� �� ������
	//Called from Character class when firing weapon
	void DecrementAmmo();

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }

	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

	void ReloadAmmo(int32 Amount);

	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	
	FORCEINLINE void SetMovingClip(bool Move) { bMoovingClip = Move; }

	//true - ���� ������� �����, ����� false
	//return true if magazine full, else - false
	bool ClipIsFull();
};
