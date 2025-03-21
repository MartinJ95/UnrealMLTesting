#include "Checkpoint.h"
#include "Components/SphereComponent.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(Checkpoint, "Checkpoint", "Checkpoint Tag");

ACheckPoint::ACheckPoint()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SphereCol = CreateDefaultSubobject<USphereComponent>("SphereCol");
	SphereCol->SetupAttachment(Root);
	Tags.Add("Checkpoint");

	SphereCol->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereCol->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SphereCol->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}