#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Checkpoint.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Checkpoint);

class USphereComponent;

UCLASS()
class ACheckPoint : public AActor
{
	GENERATED_BODY()
public:
	ACheckPoint();
	void NotifyActorBeginOverlap(AActor* OtherActor) override final;
public:
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCol;
};