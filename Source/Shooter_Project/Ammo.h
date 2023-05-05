#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Ammo.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTER_PROJECT_API AAmmo : public AItem
{
	GENERATED_BODY()
		
public:
	AAmmo();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	//Переопределение функции SetItemProperties для того что бы можно было устанавливать свойста AmmoMesh
	//Override of SetItemProperties so we can set Ammomesh properties
	virtual void SetItemProperties(EItemState State) override;


	UFUNCTION()
	void AmmoSphereOverlaped(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtheBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
private:

	//Меш для поднимаемых патронов
	//Mesh for the ammo pickup
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AmmoMesh;

	//Тип патронов для патронов
	//Ammo type for the ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	//Текстура для иконки патронов
	//Texture fo the Ammo icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	UTexture2D* AmmoIconTexture;

	//Сфера пересечения для подъёма патронов
	//Pverlap sphere for pickup the Ammo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AmmoCollisionSphere;

public:

	FORCEINLINE UStaticMeshComponent* GetAmmoMesh() const { return AmmoMesh; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
};
