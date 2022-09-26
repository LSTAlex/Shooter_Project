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

	//¬ызываетс€ дл€ ввода вперЄд/назад
	//Called for forwards/backwards input
	void MoveForward(float Value);

	//¬ызываетс€ дл€ ввода в бок
	//Called for side to side input
	void MoveRight(float Value);

	//вызываетс€ посредством ввода дл€ вращени€ с заданной скоростью
	//Called via input to turn at a given rate
	//@param Rate this is a normalized rate. 1 = 100% of disired turn rate
	void TurnAtRate(float Rate);

	//вызываетс€ посредством ввода дл€ поворота камеры вверх/вниз с заданной скоростью
	//Called via input to look up/down at a given rate
	//@param Rate this is a normalized rate. 1 = 100% of disired turn rate
	void LookUpAtRate(float Rate);

	//вращение контроллера на основе движени€ мыши по ’
	//Rotate controller based on mouse X movement
	//@param Value The inpun value from mouse movement
	void Turn(float Value);

	//вращение контроллера на основе движени€ мыши по Y
	//Rotate controller based on mouse Y movement
	//@param Value The inpun value from mouse movement
	void LookUp(float Value);
	
	//вызываетс€, когда нажата кнопка стрельбы
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

	//устанавливает базовую скоость вращени€ в зависимости от прицеливани€
	//Set BaseTurnRate and BaseLookUpRate based on aiming
	void SetLookRates();



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

	//—амера, следующа€ за игроком
	//Camera that follows the character
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


	//Ѕазовое значение дл€ поворота в градусах/сек
	//Base turn rate, in deg/sec.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	//Ѕазовое значение дл€ взгл€да вверх/вниз в градусах/сек
	//Base look up/down rate, in deg/sec.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	//рандомный звук выстрела из оружи€
	//Randomized gunshoot sound cue
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	//вспышка, призванна€ в сокете ствола
	//Flash spawned at BarrelSocket
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzelFlash;

	//монтаж дл€ стрельбы из оружи€
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

	//стандартное значение пол€ обзора камеры
	//Default camera field of view value
	float CameraDefaultFOV;
	//значение пол€ обзора, когда сделано приблежение
	float CameraZoomedFOV;

	//текущее поле обзора в данном кадре
	//current field of view this frame
	float CameraCurrentFOV;

	//скорость интерпол€ции приблежени€ камеры при прицеливании
	//Interpolation speed for zooming aiming
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpspeed;
#pragma endregion reg1


	//скорость вращени€ по вертикали не в прицеливании
	//Turn rate while not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipTurnRate;
	//скорость вращени€ по горизонталине в прицеливании
	//Look up rate when not aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipLookUpRate;
	//скорость вращени€ по горизонтали в прицеливании
	//Turn rate when aiming
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimingTurnRate;
	//скорость вращени€ по вертикали в прицеливании
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

public:

	//¬озвращает субобъект CameraBoom
	//Returns CameraBoom subobject
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//возвращает FollowCamera
	//Returns FollowCamera subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool GetAiming() const { return bAiming; }
};
