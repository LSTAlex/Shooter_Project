// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8
{
	EOS_Aiming UMETA(DisplayName = "Aiming"),
	EOS_Hip UMETA(DisplayName = "Hip"),
	EOS_Reloading UMETA(DisplayName = "Reloading"),
	EOS_InAir UMETA(DisplayName = "InAir"),

	EOS_MAX UMETA(DisplayName = "DefaultMAX"),
};

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

	//Выполняет вычисление для наклона во время бега
	//Handle calculations for leaning while running
	void Lean(float DeltaTime);

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
	float TIPCharacterYaw;

	//Рысканье персонажа в прошлом кадре
	//Yaw of the character the previous frame
	float TIPCharacterYawLastFrame;
	
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

	//состояние смещения используется для определения какое состояние использовать
	//Offset state; used to determine which Aim Offset to use
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateaccess = "true"))
	EOffsetState OffsetState;

	//Рысканье персонажа в этом кадре
	//Сharacter Yaw this frame
	FRotator CharacterRotation;

	//Рысканье персонажа в прошлом кадре
	//Yaw of the character the previous frame
	FRotator CharacterRotationLastFrame;

	//Дельта рысканья, используемая для наклона в пространстве смешивания бега
	//Yaw delta used for leaning in the running blendspace
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lean, meta =(AllowPrivateAccess = "true"))
	float YawDelta;
};
