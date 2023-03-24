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

	//��������� ����������� �������� �� �����
	//Handle turning in place variables
	void TurnInPlace();

	//��������� ���������� ��� ������� �� ����� ����
	//Handle calculations for leaning while running
	void Lean(float DeltaTime);

private:

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter;

	//�������� ���������
	//The speed of the character
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;
	//��������� �� � �������
	//Whether or not the character is in the air
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	//���������� �� ��������
	//Whether or not the character is moving
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	//�������� ��������, ������� ������������ ��� �������
	//Offset yaw used for strafing
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	//�������� �������� � ����� �� ���� ��� �� ��������� ���������
	//Offset yaw the frame before we stopped moving 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	//
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	//�������� ��������� �� �����������
	//Yaw of the character this frame
	float TIPCharacterYaw;

	//�������� ��������� � ������� �����
	//Yaw of the character the previous frame
	float TIPCharacterYawLastFrame;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateaccess =  "true"))
	float RootYawOffset;

	//�������� �������� � ���� �����
	//Rotation curve value this frame
	float RotationCurve;

	//�������� �������� ������ � ������� �����
	//Rotation curve value last frame
	float RotationCurveLastFrame;

	//�������� ������ ���������
	//get pitch character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateaccess = "true"))
	float Pitch;

	//�������� true ��� �����������, ������������ ��� �������������� �������� ������������ ��� �����������
	//True when reloading, used to prevent Aim Offset while reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateaccess = "true"))
	bool bReloading;

	//��������� �������� ������������ ��� ����������� ����� ��������� ������������
	//Offset state; used to determine which Aim Offset to use
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateaccess = "true"))
	EOffsetState OffsetState;

	//�������� ��������� � ���� �����
	//�haracter Yaw this frame
	FRotator CharacterRotation;

	//�������� ��������� � ������� �����
	//Yaw of the character the previous frame
	FRotator CharacterRotationLastFrame;

	//������ ��������, ������������ ��� ������� � ������������ ���������� ����
	//Yaw delta used for leaning in the running blendspace
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lean, meta =(AllowPrivateAccess = "true"))
	float YawDelta;
};
