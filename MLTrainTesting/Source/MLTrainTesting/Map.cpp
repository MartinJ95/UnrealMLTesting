#include "Map.h"

AMapTile::AMapTile()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetupAttachment(Root);
}

void AMapManager::GenerateMap()
{
	if (Map[0][0] != nullptr)
		return;

	for (int i = 0; i < MapSize; i++)
	{
		for (int j = 0; j < MapSize; j++)
		{
			assert(Mesh != nullptr);
			float MeshScale = Mesh->GetBounds().SphereRadius * 1.2;
			Map[i][j] = GetWorld()->SpawnActor<AMapTile>();
			Map[i][j]->SetActorLocation(FVector(j * TileSize * MeshScale, i * TileSize * MeshScale, 0));
			Map[i][j]->SetActorScale3D(FVector(TileSize, TileSize, 1));
			Map[i][j]->GetComponentByClass<UStaticMeshComponent>()->SetStaticMesh(Mesh);
			Map[i][j]->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, *Mats.Find(Map[i][j]->Type));
		}
	}
}

void AMapManager::DeleteMap()
{
	for (int i = 0; i < MapSize; i++)
	{
		for (int j = 0; j < MapSize; j++)
		{
			GetWorld()->DestroyActor(Map[i][j]);
			Map[i][j] = nullptr;
		}
	}
}

void AMapManager::GenPath()
{
	Path.PathDirs.Empty();
	Path.Length = 0;

	for (int i = 0; i < MapSize; i++)
	{
		for (int j = 0; j < MapSize; j++)
		{
			Map[i][j]->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, *Mats.Find(TileType::Grass));
		}
	}

	while (Path.Length < MapSize)
	{
		int stride = FMath::RandRange(1, MapSize - Path.Length);
		int pos = FMath::RandRange(0, MapSize - 1);
		Path.Length += stride;
		Path.PathDirs.Emplace(pos, stride);
	}

	int current = 0;
	for (int i = 0; i < Path.PathDirs.Num(); i++)
	{
		for (int j = 0; j < Path.PathDirs[i].Value; j++)
		{
			Map[Path.PathDirs[i].Key][current + j]->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, *Mats.Find(TileType::Path));
		}

		if (i == Path.PathDirs.Num() - 1)
			continue;

		current += Path.PathDirs[i].Value;

		int verticalDir = (Path.PathDirs[i + 1].Key - Path.PathDirs[i].Key) < 0 ? -1 : 1;
		for (int j = 0; j < FMath::Abs(Path.PathDirs[i + 1].Key - Path.PathDirs[i].Key); ++j)
		{
			Map[Path.PathDirs[i].Key + verticalDir * j][current]->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, *Mats.Find(TileType::Path));
		}
	}
}

void AMapManager::DeletePath()
{
	Path.PathDirs.Empty();
	Path.Length = 0;
	for (int i = 0; i < MapSize; i++)
	{
		for (int j = 0; j < MapSize; j++)
		{
			Map[i][j]->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, *Mats.Find(TileType::Null));
		}
	}
}