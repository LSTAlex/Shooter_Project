// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),	
	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),
	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FItemRarityTable: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* IconColorBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CustomDepthStencil;

};


UCLASS()
class SHOOTER_PROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//���������� ��� ����������� AreaSphere
	//Called when overlapping AreaSphere
	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtheBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	//���������� ��� ���������� ����������� AreaSphere
	//Called when end overlapping AreaSphere
	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtheBodyIndex);

	//������������� ������ ActiveStars � ����������� �� ��������
	//Set the ActiveStars array of bools based on rarity
	void SetActiveStars();

	//������������� �������� ���������� �������� � ����������� �� ���������
	//Sets properties of the items components based on State
	virtual void SetItemProperties(EItemState State);

	//���������� ����� ItemInterpTimer �������������
	//Called when ItemInterpTimer is finished
	void FinishInterping();

	//��������� ������������� ��������, ������� ��������� � ��������� EquipInterping
	//Handles item interpolation when in the EquipInterping
	void ItemInterp(float DeltaTime);

	//��������� ��������� ������������ �� ������ ���� ��������
	//Get interp location based on the item type
	FVector GetInterpLocation();

	void PlayPickupSound();

	virtual void InitializeCustomDepth();

	virtual void OnConstruction(const FTransform& Transform) override;

	void EnableGlowMaterial();

	void UpdatePulse();

	void ResetPulseTimer();

	void StartPulseTimer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

#pragma region reg1
	//��������� ��� ��� ��������
	//Skeletal mesh for the item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh;

	//�������� ����������� ���������� CollisionBox ��� �� ���������� ������� HUD
	//Line trace collides wuth box to show HUD widgets
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	//������ ������������ ����, ��� ���� ������, ����� ����� ������ �� �������
	//Popup widget for when the player looks at the item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;

	//�������� ����������� ����� �����������
	//Enable item tracing when overlapped
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere;

	//��������, ������� ���������� �� PickupWidget
	//The name which appears on the PickupWidget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	//ItemCount (������� � �.�.)
	//ItemCount (ammo, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCount;

	//�������� �������� ���������� ���-�� ���� � PickupWidget
	//Item rarity - determines number of stars in PickupWidget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

	//��������� ��������
	//State of the Items
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	//����� ������ ��� ������������� � ����������� Z �������������� ��������
	//The curve asset to use for the items's Z location when interping
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* ItemZCurve;

	//��������� �������������� ��� ������ ������������
	//Starting location when interping begins
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector ItemInterpStartLocation;

	//�������������� ����� ������� ��� ������������
	//Target interp location in front of the camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FVector CameraTargetLocation;

	//True �� ����� ������������
	//True when interping
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bInterping;

	//��������������� ��� ������ ������������
	//Plays when we start interping
	FTimerHandle ItemInterpTimer;

	//����������������� ������ � �������
	//Duration of the curve and timer
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float ZCurveTime;

	//��������� �� ����������
	//Pointer to the Character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* Character;

	//X � Y ��� �������� � �������� ������������ � ��������� EquipInterping
	//X and Y for the Item while interping in the EquipInterping state
	float ItemInterpX;
	float ItemInterpY;

	//�������� ��������� �������� ����� ������� � �������������� ������������ ��������
	//Initial Yaw offset between the camera and the interping item
	float InterpInitialYawOffset;

	//������ ��� ��������������� ��������
	//Curve for scale Item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ItemScaleCurve;

	//���� ��������������� ��� �������� ��������
	//Sound played when item is picked up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* PickuoSound;

	//���� ��������������� ��� �������������� ��������
	//Sound played when the item is equipped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* EquipSound;

	//������������ � ������ ���������
	//Enum for the type of item this Item is
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	//������ ����� ������������, ���� ������������� ���� �������
	//Index of the interp location this item is interping to
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 InterpLocIndex;

	//������ ��� ����������, ������� ����� �������� � ���� ���������� ���������
	//Index for the material like to change at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 MaterialIndex;

	//������������ ���������, ������� ����� ������ � ���� ����������
	//Dynamic instance that can change at runtime
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	//��������� ��������� ������������ � ������������ ����������
	//Material instance used with the dynamic material instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* MaterialInstance;

	bool bCanChangeCustomDepth;

	//������ ��� ���������� ����������� ������������� ���������
	//Curve to drive the dynamic material parameters
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UCurveVector* PulseCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	UCurveVector* InterpPulseCurve;

	FTimerHandle PulseTimer;

	//����� ��� ������� ���������
	//Time for the PulseTimer
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float PulseCurveTime;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float GlowAmount;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float FresnelExponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float FresnelReflectFraction;

	//��� ��� ����� �������� � ���������
	//Background for this item  in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;

	//������ ��� ����� �������� � ���������
	//Icon for this item  in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItem;
#pragma endregion reg1
		
	//������ �������� ��� ����� �������� � ���������
	//Icon ammo for this item  in the inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UTexture2D* AmmoIcon;

	//���� � ������� ���������
	//Slot in the inventory array
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;

	//����� �������� true, ����� ��������� ��������� �����
	//True when the character inventory is full
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	bool bCharacterInventoryFull;

	//������� ������ �������� ��������
	//Item rarity data table
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemRarityDataTable;

	//���� � ��������� ������
	//Color in the glow material
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor GlowColor;

	//������� ���� �� ������� ������������
	//Light color in the pickup widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor LightColor;

	//Ҹ���� ���� �� ������� ������������
	//Dark color in the pickup widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	FLinearColor DarkColor;

	//���-�� ���� � ������� ������������ ��������
	//Number of stars the pickup widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	int32 NumberOfStars;

	//������ ���� ��� ���������
	//Background icon for the inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rarity, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconColorBackground;

public:

	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE EItemState GetItemState() const { return ItemState; }
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE USoundCue* GetPickuoSound() const { return PickuoSound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }
	FORCEINLINE void SetPickuoSound(USoundCue* Sound) { PickuoSound = Sound; }
	FORCEINLINE void SetEquipSound(USoundCue* Sound) { EquipSound = Sound; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	FORCEINLINE int32 GetISlotIndex() const { return SlotIndex; }
	FORCEINLINE void SetISlotIndex(int32 Index) { SlotIndex = Index; }
	FORCEINLINE void SetCharacterInventoryFull(bool bFull) { bCharacterInventoryFull = bFull; }
	FORCEINLINE void SetItemName(FString Name) { ItemName = Name; }
	FORCEINLINE void SetItemIcon(UTexture2D* Icon) { IconItem = Icon; }
	FORCEINLINE void SetAmmoIcon(UTexture2D* Icon) { AmmoIcon = Icon; }

	//������� �� ������ AShooterCharacter
	//Called from the AShooterCharacter
	void StartItemCurve(AShooterCharacter* Char);
	void SetItemState(EItemState State);
	//���������� � ������� AShootercharacter::GetPickupItem
	//Called in AShootercharacter::GetPickupItem
	void PlayEquipSound();

	virtual void EnableCustomDepth();

	virtual void DisableCustomDepth();
	void DisableGlowMaterial();

	
};
