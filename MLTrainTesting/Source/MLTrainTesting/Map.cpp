#include "Map.h"
#include "Checkpoint.h"

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
	DeletePath();

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
	DeletePath();

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
		Path.CheckPoints.Add(GetWorld()->SpawnActor<ACheckPoint>(Map[pos][Path.Length]->GetActorLocation() + FVector(0, 0, 100), FRotator()));
		Path.CheckPoints.Add(GetWorld()->SpawnActor<ACheckPoint>(Map[pos][FMath::Min(Path.Length + stride, MapSize-1)]->GetActorLocation() + FVector(0, 0, 100), FRotator()));
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
	for (int i = 0; i < Path.CheckPoints.Num(); i++)
	{
		GetWorld()->DestroyActor(Path.CheckPoints[i]);
	}
	Path.CheckPoints.Empty();
}

bool AMapManager::IsOnPath(const int X, const int Y) const
{
	int current = 0;

	for (int i = 0; i < Path.PathDirs.Num(); i++)
	{
		if (X > current + Path.PathDirs[i].Value)
			continue;

		if (Y == Path.PathDirs[i].Key)
			return true;
	}
	return false;
}

const TArray<TPair<int, int>> AMapManager::GetPath() const
{
	TArray<TPair<int, int>> path;

	int current = 0;
	for (int i = 0; i < Path.PathDirs.Num(); i++)
	{
		for (int j = 0; j < Path.PathDirs[i].Value; j++)
		{
			path.Emplace(Path.PathDirs[i].Key, current + j);
		}

		if (i == Path.PathDirs.Num() - 1)
			continue;

		current += Path.PathDirs[i].Value;

		int verticalDir = (Path.PathDirs[i + 1].Key - Path.PathDirs[i].Key) < 0 ? -1 : 1;
		for (int j = 0; j < FMath::Abs(Path.PathDirs[i + 1].Key - Path.PathDirs[i].Key); ++j)
		{
			path.Emplace(Path.PathDirs[i].Key + verticalDir * j, current);
		}
	}
	return path;
}
