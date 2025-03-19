#include "Map.h"

AMapTile::AMapTile()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetupAttachment(Root);

	
}
