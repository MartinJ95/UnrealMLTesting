#include "Tower.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "Game.h"
#include "Components/SphereComponent.h"

ATower::ATower()
{
	Detection = CreateDefaultSubobject<USphereComponent>("sphere");

	Detection->SetupAttachment(GetRootComponent());

	Detection->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	Detection->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Detection->SetSphereRadius(Range);

	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &ATower::Attack, 2.2f, true);
}

void ATower::Attack()
{
	if (EnemiesInRange.Num() == 0)
		return;

	ATowerDefense* GameMode = Cast<ATowerDefense>(GetWorld()->GetAuthGameMode());

	GameMode->OnEnemyKilled(EnemiesInRange.Pop());
}

void ATower::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AEnemy* e = Cast<AEnemy>(OtherActor);
	if (!e)
		return;

	EnemiesInRange.Add(e);
}

void ATower::NotifyActorEndOverlap(AActor* OtherActor)
{
	AEnemy* e = Cast<AEnemy>(OtherActor);
	if (!e)
		return;

	EnemiesInRange.Remove(e);
}
