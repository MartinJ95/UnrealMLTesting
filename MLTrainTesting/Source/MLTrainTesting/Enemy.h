#pragma once

#include "CoreMinimal.h"
#include "Enemy.generated.h"

UCLASS()
class AEnemy : public APawn
{
	GENERATED_BODY()
public:
	AEnemy();
public:
	UPROPERTY(EditAnywhere)
	int currentStep{ 0 };
};