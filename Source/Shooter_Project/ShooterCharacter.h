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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//���������� ��� ����� �����/�����
	//Called for forwards/backwards input
	void MoveForward(float Value);

	//���������� ��� ����� � ���
	//Called for side to side input
	void MoveRight(float Value);

	//���������� ����������� ����� ��� �������� � �������� ���������
	//Called via input to turn at a given rate
	//@param Rate this is a normalized rate. 1 = 100% of disired turn rate
	void TurnAtRate(float Rate);

	//���������� ����������� ����� ��� �������� ������ �����/���� � �������� ���������
	//Called via input to look up/down at a given rate
	//@param Rate this is a normalized rate. 1 = 100% of disired turn rate
	void LookUpAtRate(float Rate);
	
	//����������, ����� ������ ������ ��������
	//called when the Fire Button is pressed
	void FireWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//CameraBoom ������������� ������ �� ���������� 
	//CameraBoom positioning the camera behind the character
	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//������, ��������� �� �������
	//Camera that follows the character
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


	//������� �������� ��� �������� � ��������/���
	//Base turn rate, in deg/sec.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	//������� �������� ��� ������� �����/���� � ��������/���
	//Base look up/down rate, in deg/sec.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	//��������� ���� �������� �� ������
	//Randomized gunshoot sound cue
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	//�������, ���������� � ������ ������
	//Flash spawned at BarrelSocket
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzelFlash;

public:

	//���������� ��������� CameraBoom
	//Returns CameraBoom subobject
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//���������� FollowCamera
	//Returns FollowCamera subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
