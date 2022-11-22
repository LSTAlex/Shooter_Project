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

	//�������� ����������� �� ������ �������� ���� �� �
	//Rotate controller based on mouse X movement
	//@param Value The inpun value from mouse movement
	void Turn(float Value);

	//�������� ����������� �� ������ �������� ���� �� Y
	//Rotate controller based on mouse Y movement
	//@param Value The inpun value from mouse movement
	void LookUp(float Value);
	
	//����������, ����� ������ ������ ��������
	//called when the Fire Button is pressed
	void FireWeapon();

	//�������� ���������� ����� ������� �����
	//get end beam location
	bool GetBeamEndLocation(const FVector &MuzzelSocketLocation, FVector &OutBeamLocation);

	void AimingButtonPressed();
	void AimingButtonReleased();

	//������� ������������
	//smooth aiming
	void CameraInterpZoom(float DeltaTime);

	//������������� ������� ������� �������� � ����������� �� ������������
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

	//�������� ����������� ��� ��������� ��� ������������
	//Line trace for items under the crosshairs
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector OutHitLocation);

	//����������� ��� ���������, � ������ ���� OverlappedItemCount <0
	//Trace for items if OverlappedItemCount >0
	void TraceForItems();

	//���� ������ � ����������� ��� � ����
	//Takes a weapon and attaches it to the mesh
	void EquipWeapon(AWeapon* WeaponToEquip);

	//���������� ������ � ��������� ��� ������ �� �����
	//Detach weapon and let it fall to the ground
	void DropWeapon();

	void SelectButtonPressed();

	void SelectButtonReleased();

	//���������� ������� ������������� ������ � ��������� TraceHitItem
	//Drops currently equipped weapon and equips TraceHitItem
	void SwapWeapon(AWeapon* WeaponToSwap);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

#pragma region reg1
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

	//������ ��� �������� �� ������
	//Montage for firing the weapon
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;

	//�������, ����������� ��� ��������� ����
	//Particles spawned upon bullet impact
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	//������ ���� �� ����
	//Smoke trail for bullet
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;


	//true ��� ������������ 
	//true when aiming
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	//����������� �������� ���� ������ ������
	//Default camera field of view value
	float CameraDefaultFOV;
	//�������� ���� ������, ����� ������� �����������
	float CameraZoomedFOV;

	//������� ���� ������ � ������ �����
	//current field of view this frame
	float CameraCurrentFOV;

	//�������� ������������ ����������� ������ ��� ������������
	//Interpolation speed for zooming aiming
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpspeed;

	//�������� �������� �� ��������� �� � ������������
	//Turn rate while not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;
	//�������� �������� �� ������������� � ������������
	//Look up rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;
	//�������� �������� �� ����������� � ������������
	//Turn rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;
	//�������� �������� �� ��������� � ������������
	//Look up rate when aimng
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingLookUpRate;

	//��������� ������ ��� ������������� ���� �� � ������������
	//Scale factor for mouse look sensitivity. Turn rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0",UIMin = "0.0", UIMax = "1.0"))
	float MouseHipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;

	//��������� ������ ��� ������������� ����  � ������������
	//Scale factor for mouse look sensitivity. LookUp rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;

	//���������� ������� �����������
	//defines the crosshair spread
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;
	//��������� �������� ��� �������� �����������
	//Velocity component for crosshairs spread
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairVelosityFactor;
	//��������� � ������� ��� �������� �����������
	//in air component for crosshairs spread
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAimFactor;
	//��������� �������� ��� �������� �����������
	//Shooting component for crosshairs spread
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;


	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	//����� ������ ����
	//Left mouse button bressed
	bool bFireButtonPressed;

	//����� �������� true - ����� ����� ��������,false - ����� ������� ����������� ��������
	//True when we can fire. False when waiting for the timer
	bool bShouldFire;

	//���� ��������������� ����
	//rate of automatic gun fire
	float AutomaticFireRate;

	//������������� ������ ����� ����������
	//sets a timer between gunshots
	FTimerHandle AutoFireTimer;

	//����� �������� true ���� ����� ������ ����������� ������ ���� ��� ����������� ���������
	//True if we should trace every fame for items
	bool bShouldTraceForItems;
#pragma endregion reg1

	//���-�� ������������ ���������
	//Number of overlapped AItems
	int8 OverlappedItemConut;

public:

	//���������� ��������� CameraBoom
	//Returns CameraBoom subobject
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//���������� FollowCamera
	//Returns FollowCamera subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	//���������� bAiming
	//Returns bAiming
	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const { return CrosshairSpreadMultiplier; }

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemConut; }

	//����������� ��� ��������� OverlappedItemConut � ��������� bShouldTraceForItems
	//Adds/substracts to/from OverlappedItemConut and updates bShouldTraceForItems
	void IncrementOverlappedItemCount(int8 Amount);
};
