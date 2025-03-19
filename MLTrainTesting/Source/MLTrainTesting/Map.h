#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Components/StaticMeshComponent.h"
#include <array>
#include <assert.h>

#include "Map.generated.h"

constexpr int MapSize = 10;

UENUM(BlueprintType)
enum  TileType : uint8
{
	Null = 0,
	Grass = 1,
	Path = 2
};

UCLASS()
class AMapTile : public AActor
{
	GENERATED_BODY()
public:
	AMapTile();
public:
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<TileType> Type{ TileType::Null };
};


UCLASS()
class AMapManager : public AActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateMap()
	{
		if (Map[0][0] != nullptr)
			return;

		for (int i = 0; i < MapSize; i++)
		{
			for (int j = 0; j < MapSize; j++)
			{
				assert(Mesh != nullptr);
				float MeshScale = Mesh->GetBounds().SphereRadius;
				Map[i][j] = GetWorld()->SpawnActor<AMapTile>();
				Map[i][j]->SetActorLocation(FVector(j * TileSize * MeshScale, i * TileSize * MeshScale, 0));
				Map[i][j]->SetActorScale3D(FVector(TileSize, TileSize, 1));
				Map[i][j]->GetComponentByClass<UStaticMeshComponent>()->SetStaticMesh(Mesh);
				Map[i][j]->GetComponentByClass<UStaticMeshComponent>()->SetMaterial(0, *Mats.Find(Map[i][j]->Type));
			}
		}
	}
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DeleteMap()
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
public:
	UPROPERTY(EditAnywhere)
	int TileSize{1};
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh{nullptr};
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<TileType>, UMaterialInstance*> Mats;
protected:
	std::array<std::array<AMapTile*, MapSize>, MapSize> Map;
};
