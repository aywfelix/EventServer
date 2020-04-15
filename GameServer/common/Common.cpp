#include "Common.h"

float GetDistance(const WorldPos& posA, const WorldPos& posB)
{
	float xDis = posA.m_x - posB.m_x;
	float zDis = posA.m_x - posB.m_z;
	return std::sqrt(xDis * xDis + zDis * zDis);
}