#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class AEnemy : public ACharacter
{
	GENERATED_BODY()
public:
	AEnemy();
public:
	UPROPERTY(EditAnywhere)
	int currentStep{ 0 };
};