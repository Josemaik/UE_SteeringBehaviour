// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCell.h"
#include "GridActor.generated.h"

UCLASS()
class MPV_PRACTICAS_API AGridActor : public AActor
{
	GENERATED_BODY()

	//8x8 = 64 lineal elements
	TArray<FGridCell> GridCells;

	UPROPERTY(EditAnywhere)
	int GridSize = 8;

	UPROPERTY(EditAnywhere)
	float CellSize = 100.f;
	
	void LoadGrifFromFile(const FString& MapPath, const FString& CostMapPath);
	
	//Position of the top left corner first cell position
	//const FVector GridPosition = FVector(-381.650517,0.000000,270.000000);
	
public:	
	// Sets default values for this actor's properties
	AGridActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
