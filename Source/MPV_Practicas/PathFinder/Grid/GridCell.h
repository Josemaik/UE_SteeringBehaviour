#pragma once

struct FGridCell
{
	FVector Center;      // Posición en el mundo (centro de la celda)
	bool bPassable;      // ¿Es transitable?
	//char TerrainType;    // Letra del tipo de terreno (A, B, C...)
	int Cost;            // Coste de movimiento
};