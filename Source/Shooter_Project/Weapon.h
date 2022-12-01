// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_PROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:
	
	void StopFalling();

private:

	FTimerHandle ThrowWeaponTimer;

	float ThrowWeaponTime;

	bool bFalling;

	//Кол-во патрон для этого оружия
	//Ammo count for this Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

public:

	//Добавляет импульс к оружию
	//Add impulse to the weapon
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	//Вызывается из класса персонажа при стрельбе из оружия
	//Called from Character class when firing weapon
	void DecrementAmmo();
};
