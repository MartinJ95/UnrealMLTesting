#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Components/StaticMeshComponent.h"
#include <array>
#include <assert.h>

#include "Map.generated.h"

class ACheckPoint;

constexpr int MapSize = 25;

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

USTRUCT(BlueprintType)
struct FPath
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Length{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ACheckPoint*> CheckPoints;
	TArray<TPair<int, int>> PathDirs;
};

UCLASS(BlueprintType)
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
	UFUNCTION(BlueprintCallable)
	bool IsOnPath(const int X, const int Y) const;
	const TArray<TPair<int, int>> GetPath() const;
public:
	UPROPERTY(EditAnywhere)
	int TileSize{1};
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh{nullptr};
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<TileType>, UMaterialInstance*> Mats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Size{ MapSize };
public:
	std::array<std::array<AMapTile*, MapSize>, MapSize> Map;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPath Path;
};
