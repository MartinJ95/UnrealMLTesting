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
	UPROPERTY(EditAnywhere)
	TArray<AActor*> Checkpoints;
};

USTRUCT()
struct FPath
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int Length{ 0 };
	TArray<TPair<int, int>> PathDirs;
};

UCLASS()
class AMapManager : public AActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateMap();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DeleteMap();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenPath();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DeletePath();
public:
	UPROPERTY(EditAnywhere)
	int TileSize{1};
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh{nullptr};
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<TileType>, UMaterialInstance*> Mats;
protected:
	std::array<std::array<AMapTile*, MapSize>, MapSize> Map;
	UPROPERTY(EditAnywhere)
	FPath Path;
};
