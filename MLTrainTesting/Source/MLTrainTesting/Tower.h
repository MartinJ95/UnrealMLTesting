#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tower.generated.h"

struct FTimerHandle;
class AEnemy;
class USphereComponent;

UCLASS()
class ATower : public ACharacter
{
	GENERATED_BODY()
public:
	ATower();
	void BeginPlay() override final;
	UFUNCTION(BlueprintCallable)
	void Attack();
	void NotifyActorBeginOverlap(AActor* OtherActor) override final;
	void NotifyActorEndOverlap(AActor* OtherActor) override final;
public:
	UPROPERTY(EditAnywhere)
	USphereComponent* Detection;
	UPROPERTY(EditAnywhere)
	float Range {500.f};
	UPROPERTY(EditAnywhere)
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere);
	TArray<AEnemy*> EnemiesInRange;
};