#pragma once

#include <math.h>
#include "Common.pb.h"

// 地图坐标处理（2D）
class WorldPos
{
public:
	float m_x, m_z; // 依据 unity3d 坐标系

	WorldPos() :m_x(0.0f), m_z(0.0f) {}
	WorldPos(float x, float z) :m_x(x), m_z(z) {}
	WorldPos(const Vector3& v3) :m_x(v3.x()), m_z(v3.z()) {}

	void Clear() { m_x = 0.0f; m_z = 0.0f; }

	WorldPos& operator=(const WorldPos& pos)
	{
		this->m_x = pos.m_x;
		this->m_z = pos.m_z;
		return *this;
	}
	WorldPos& operator=(const WorldPos* pos)
	{
		this->m_x = pos->m_x;
		this->m_z = pos->m_z;
		return *this;
	}
	bool operator==(const WorldPos& pos)
	{
		return (fabs(m_x - pos.m_x) + fabs(m_z - pos.m_z)) < 0.01f;
	}
	bool operator!=(const WorldPos& pos)
	{
		return (fabs(m_x - pos.m_x) + fabs(m_z - pos.m_z)) >= 0.01f;
	}
	WorldPos operator+(const WorldPos& pos)
	{
		return WorldPos(this->m_x + pos.m_x, this->m_z + pos.m_z);
	}
	WorldPos& operator+=(const WorldPos& pos)
	{
		this->m_x += pos.m_x;
		this->m_z += pos.m_z;
		return *this;
	}
	WorldPos operator+(float v)
	{
		return WorldPos(this->m_x + v, this->m_z + v);
	}
	WorldPos& operator+=(float v)
	{
		this->m_x += v;
		this->m_z += v;
		return *this;
	}
	WorldPos operator-(const WorldPos& pos)
	{
		return WorldPos(this->m_x - pos.m_x, this->m_z - pos.m_z);
	}
	WorldPos& operator-=(const WorldPos& pos)
	{
		this->m_x -= pos.m_x;
		this->m_z -= pos.m_z;
		return *this;
	}

	Vector3 ToV3()
	{
		Vector3 v3;
		v3.set_x(m_x);
		v3.set_y(m_z);
		return v3;
	}
};

float GetDistance(const WorldPos& posA, const WorldPos& posB);