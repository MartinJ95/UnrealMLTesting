#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "Game.generated.h"

class AMapManager;
class AEnemy;
class ATower;

UENUM()
enum GameResult : uint8
{
	None,
	Won,
	Lost
};

UCLASS(BlueprintType)
class ATowerDefense : public AGameModeBase
{
	GENERATED_BODY()
public:
	void StartPlay() override final;
	UFUNCTION(BlueprintCallable)
	void StartUpGame();
	UFUNCTION(BlueprintCallable)
	void EndGame();
	UFUNCTION(BlueprintCallable)
	void SpawnTower(const int X, const int Y);
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	UFUNCTION(BlueprintCallable)
	void EnemyReachCheckpoint(AEnemy* Enemy);
	UFUNCTION(BlueprintCallable)
	void AddMoney();
	UFUNCTION(BlueprintCallable)
	void OnEnemyKilled(AEnemy* Enemy);
	UFUNCTION(BlueprintCallable)
	float ProvideDistanceToCheckpointScore();
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<GameResult> GetResult();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATower> TowerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> EnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMapManager* Map;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemy*> Enemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ATower*> Towers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Money{100};
	UPROPERTY(EditAnywhere)
	int CurrentSpawned{ 0 };
	UPROPERTY(EditAnywhere)
	FTimerHandle MoneyTimer;
	UPROPERTY(EditAnywhere)
	FTimerHandle SpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATower* LastTower{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<GameResult> Result{ GameResult::None };
};