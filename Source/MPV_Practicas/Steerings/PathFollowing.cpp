#include "PathFollowing.h"
#include "DrawDebugHelpers.h"
#include "MPV_Practicas/AICharacter.h"
#include "SeekSteering.h"

PathFollowing::PathFollowing(AAICharacter* character, const TArray<FVector>& inPath)
	: m_character(character), m_path(inPath), m_currentIndex(0)
{
	m_currentIndex = GetClosestIndexOnPath();
	m_seek = new SeekSteering(character);
}

PathFollowing::~PathFollowing()
{
	delete m_seek;
}

int32 PathFollowing::GetClosestIndexOnPath()
{
	float minDistSqr = FLT_MAX;
	int32 closestIndex = 0;
	FVector characterPos = m_character->GetActorLocation();

	for (int32 i = 0; i < m_path.Num(); ++i)
	{
		float distSqr = FVector::DistSquared(m_path[i], characterPos);
		if (distSqr < minDistSqr)
		{
			minDistSqr = distSqr;
			closestIndex = i;
		}
	}
	return closestIndex;
}

FVector PathFollowing::GetClosestPointOnPath()
{
	float minDistSqr = FLT_MAX;
	FVector closest;

	FVector characterPos = m_character->GetActorLocation();

	for (const FVector& point : m_path)
	{
		float distSqr = FVector::DistSquared(point, characterPos);
		if (distSqr < minDistSqr)
		{
			minDistSqr = distSqr;
			closest = point;
		}
	}

	return closest;
}

void PathFollowing::AdvanceToNextIfClose()
{
	const float threshold = m_character->GetParams().arrive_radius;

	if (m_currentIndex < m_path.Num())
	{
		if (FVector::Dist(m_character->GetActorLocation(), m_path[m_currentIndex]) < threshold)
		{
			m_currentIndex++;
			if (m_currentIndex >= m_path.Num())
			{
				m_currentIndex = 0;
			}
		}
	}
}

FSOutputSteering PathFollowing::GetSteering(float DeltaTime)
{
	FSOutputSteering steering;

	if (m_path.Num() == 0)
	{
		return steering;
	}

	if (!bInsidePath)
	{
		// --- FASE 1: ENTRADA AL PATH ---
		// Paso 1: encontrar el segmento más cercano
		float minDist = FLT_MAX;
		FVector closestPoint;
		int32 closestSegmentStart = 0;

		FVector pos = m_character->GetActorLocation();

		for (int i = 0; i < m_path.Num(); ++i)
		{
			int nextIndex = (i + 1) % m_path.Num();
			FVector A = m_path[i];
			FVector B = m_path[nextIndex];

			// Proyección de pos sobre el segmento AB
			FVector AP = pos - A;
			FVector AB = (B - A);
			float t = FVector::DotProduct(AP, AB) / FVector::DotProduct(AB, AB);
			t = FMath::Clamp(t, 0.f, 1.f);

			FVector proj = A + t * AB;
			float dist = FVector::DistSquared(proj, pos);

			if (dist < minDist)
			{
				minDist = dist;
				closestPoint = proj;
				closestSegmentStart = i;
			}
		}

		// Paso 2: calcular look ahead sobre ese segmento
		int nextIndex = (closestSegmentStart + 1) % m_path.Num();
		FVector direction = (m_path[nextIndex] - m_path[closestSegmentStart]).GetSafeNormal();
		FVector target = closestPoint + direction * m_character->GetParams().look_ahead;
		m_character->SetTargetPosition(target);
		m_target = target;

		// Si estoy suficientemente cerca, entro en el path
		if (FVector::Dist(pos, target) < m_character->GetParams().arrive_radius)
		{
			bInsidePath = true;
			m_currentIndex = (closestSegmentStart + 1) % m_path.Num();
		}

		return m_seek->GetSteering(DeltaTime);
	}
	else
	{
		// if (m_currentIndex >= m_path.Num())
		// 	m_currentIndex = 0;
		
		AdvanceToNextIfClose();
		
		// Actualiza el target del Seek con el punto actual del path
		m_character->SetTargetPosition(m_path[m_currentIndex]);
		m_target = m_path[m_currentIndex];

		// Delegar a Seek
		return m_seek->GetSteering(DeltaTime);
	}
}

void PathFollowing::DrawDebug()
{
	if (m_path.Num() < 2) return;

	for (int32 i = 0; i < m_path.Num(); ++i)
	{
		if (i < m_path.Num() - 1)
		{
			DrawDebugLine(m_character->GetWorld(), m_path[i], m_path[i + 1], FColor::Green, false, -1.f, 0, 2.f);
		}
		else
		{
			DrawDebugLine(m_character->GetWorld(), m_path[i], m_path[0], FColor::Green, false, -1.f, 0, 2.f);
		}
	}

	// Punto objetivo actual
	DrawDebugSphere(m_character->GetWorld(), m_target, 15.f, 8, FColor::Red, false, -1.f);

	if (m_seek)
	{
		m_seek->DrawDebug();
	}
}