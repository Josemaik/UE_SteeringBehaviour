// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"

#include <string>

void AGridActor::LoadGrifFromFile(const FString& MapPath, const FString& CostMapPath)
{
	FString MapData, CostData;
	if (FFileHelper::LoadFileToString(MapData, *MapPath) == false)
	{
		UE_LOG(LogTemp, Display, TEXT("Error loading map!"));
	}
	FFileHelper::LoadFileToString(CostData, *CostMapPath);

	TMap<char, int> CostTable;
	TArray<FString> CostLines;
	CostData.ParseIntoArrayLines(CostLines);

	for (const FString& Line : CostLines)
	{
		int Value = FCString::Atoi(&Line[1]);
		UE_LOG(LogTemp, Display, TEXT("Cost line %d"), Value);
		CostTable.Add(Line[0], Value);
	}

	TArray<FString> MapLines;
	MapData.ParseIntoArrayLines(MapLines);

	for (int y = 0; y < MapLines.Num(); y++)
	{
		const FString& Line = MapLines[y];
		for (int x = 0; x < MapLines[y].Len(); x++)
		{
			char TerrainChar = Line[x];
			bool bIsPassable = TerrainChar != '#';
			UE_LOG(LogTemp, Display, TEXT("Terrain%c"), TerrainChar);
			int Cost = CostTable.Contains(TerrainChar) ? CostTable[TerrainChar] : -1;
			
			FVector Center = GetActorLocation() + FVector(x * CellSize, 0.f, (GridSize - 1 - y) * CellSize);
			//Create new Cell
			FGridCell Cell;
			Cell.Center = Center;
			Cell.bPassable = bIsPassable;
			Cell.Cost = Cost;

			GridCells.Add(Cell);
		}
	}
}

// Sets default values
AGridActor::AGridActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridActor::BeginPlay()
{
	Super::BeginPlay();
	
	LoadGrifFromFile(
	   FPaths::ProjectContentDir() + "GridData/grid.txt",
	   FPaths::ProjectContentDir() + "GridData/costes.txt"
   );
}

// Called every frame
void AGridActor::Tick(float DeltaTime)
{
	for (const FGridCell& Cell : GridCells)
	{
		
		FColor Color = Cell.bPassable ? FColor::Green : FColor::Black;

		// También puedes usar Cost para codificar colores
		//if (Cell.Cost > 3) Color = FColor::Yellow;
		DrawDebugBox(GetWorld(), Cell.Center, FVector(CellSize * 0.5f,  5.f,CellSize * 0.5f), FColor::Green, true, -1, 0, 2);

		if (Cell.Cost > 3) Color = FColor::Yellow;
		
		DrawDebugPoint(GetWorld(),Cell.Center,10.f,Color);

		if (Cell.Cost > 0)
		{
			//FVector CellCemter = FVector(Cell.Center.X,0.f,Cell.Center.Z - 20.f);
			DrawDebugString(GetWorld(), Cell.Center + FVector(450, 0.f, 400.f), std::to_string(Cell.Cost).c_str(), this, Color, 0.f, true, 1.5f);
		}
	}
}

