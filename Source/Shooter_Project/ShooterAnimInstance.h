// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_PROJECT_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UShooterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

protected:

	//Управляет переменными вращения на месте
	//Handle turning in place variables
	void TurnInPlace();

private:

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter;

	//скорость персонажа
	//The speed of the character
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;
	//находится ли в воздухе
	//Whether or not the character is in the air
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	//ускоряется ли персонаж
	//Whether or not the character is moving
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	//Смещение рысканья, которое используется для стрейфа
	//Offset yaw used for strafing
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	//Смещение рысканья в кадре до того как мы перестали двигаться
	//Offset yaw the frame before we stopped moving 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	//
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	//Вращение персонажа по горизонтали
	//Yaw of the character this frame
	float CharacterYaw;

	//Рысканье персонажа в прошлом кадре
	//Yaw of the character the previous frame
	float CharacterYawLastFrame;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateaccess =  "true"))
	float RootYawOffset;

	//Значение вращения в этом кадре
	//Rotation curve value this frame
	float RotationCurve;

	//Значение вращения кривой в прошлом кадре
	//Rotation curve value last frame
	float RotationCurveLastFrame;

	//получаем наклон персонажа
	//get pitch character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateaccess = "true"))
	float Pitch;

	//Является true при перезарядке, используется для предотвращения смещения прицеливания при перезарядке
	//True when reloading, used to prevent Aim Offset while reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateaccess = "true"))
	bool bReloading;
};
