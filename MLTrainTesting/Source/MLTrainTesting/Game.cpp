#include "Game.h"
#include "Map.h"
#include "Enemy.h"
#include "GameFramework/GameModeBase.h"
#include "AIController.h"
#include "Checkpoint.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"

void ATowerDefense::StartPlay()
{
	AGameModeBase::StartPlay();
	Map = Cast<AMapManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapManager::StaticClass()));
	Map->GenerateMap();
	StartUpGame();
}

void ATowerDefense::StartUpGame()
{
	Map->GenPath();
	Money = 100;
	CurrentSpawned = 0;
	GetWorld()->GetTimerManager().SetTimer(MoneyTimer, this, &ATowerDefense::AddMoney, 10.f, true);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ATowerDefense::SpawnEnemy, 10.f, true);


}

void ATowerDefense::EndGame()
{
	for (int i = Enemies.Num(); i > 0; --i)
	{
		GetWorld()->DestroyActor(Cast<AActor>(Enemies[i-1]));
	}

	Enemies.Empty();

	for (int i = Towers.Num(); i > 0; --i)
	{
		GetWorld()->DestroyActor(Cast<AActor>(Towers[i-1]));
	}

	Towers.Empty();

	GetWorld()->GetTimerManager().ClearTimer(MoneyTimer);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);


}

void ATowerDefense::SpawnTower(const int X, const int Y)
{
	TArray<TPair<int, int>> pathPoints = Map->GetPath();

	if (Map->IsOnPath(X, Y))
		return;

	if (Money < 100)
		return;

	Towers.Add(GetWorld()->SpawnActor<ATower>(TowerClass.Get(), Map->Map[Y][X]->GetActorLocation() + FVector(0, 0, 100), FRotator()));
	Money -= 100;
	LastTower = Towers.Last();
}

void ATowerDefense::SpawnEnemy()
{
	FVector spawnLocation = Map->Map[Map->Path.PathDirs[0].Key][0]->GetActorLocation() + FVector(0, 0, 100);
	FRotator rotation;

	Enemies.Add(GetWorld()->SpawnActor<AEnemy>(EnemyClass.Get(), spawnLocation, rotation));
	if (!IsValid(Enemies.Last()))
	{
		Enemies.Pop();
		return;
	}

	CurrentSpawned++;
	if (CurrentSpawned < 10)
		return;

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
}

void ATowerDefense::EnemyReachCheckpoint(AEnemy* Enemy)
{
	Enemy->currentStep++;
	if (Enemy->currentStep == Map->Path.CheckPoints.Num())
	{
		EndGame();
		StartUpGame();
		return;
	}
	AAIController* c = Cast<AAIController>(Enemy->GetController());
	c->MoveToLocation(Map->Path.CheckPoints[Enemy->currentStep]->GetActorLocation());
}

void ATowerDefense::AddMoney()
{
	Money += 10;
}

void ATowerDefense::OnEnemyKilled(AEnemy* Enemy)
{
	Enemies.Remove(Enemy);
	GetWorld()->DestroyActor(Enemy);

	if (Enemies.Num() > 0 || GetWorld()->GetTimerManager().IsTimerActive(SpawnTimer))
		return;

	EndGame();
	StartUpGame();
}

float ATowerDefense::ProvideDistanceToCheckpointScore()
{
	if (!LastTower)
		return 0.f;

	float closestDist{ 9999999 };

	for (int i = 0; i < Map->Path.CheckPoints.Num(); i++)
	{
		if (FVector::Dist(LastTower->GetActorLocation(), Map->Path.CheckPoints[i]->GetActorLocation()) >= closestDist)
			continue;
	}

	LastTower = nullptr;
	
	float score = FMath::Min(800, FMath::Max(0, closestDist));
	
	return 800 - score - 400;
}

TEnumAsByte<GameResult> ATowerDefense::GetResult()
{
	TEnumAsByte temp = Result;
	Result = GameResult::None;
	return temp;
}
