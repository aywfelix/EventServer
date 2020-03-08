#pragma once
#include <cmath>

#define  PI 3.14159f
// 三维地图处理
class V3Map
{
public:
	float m_x, m_y, m_z;
	V3Map(){}
	V3Map(const V3Map& v3):m_x(v3.m_x),m_y(v3.m_y),m_z(v3.m_z){}
	V3Map(float x,float y, float z):m_x(x),m_y(y),m_z(z){}

	// operator
	V3Map operator-() const
	{
		return V3Map(-m_x, -m_y, -m_z);
	}

	V3Map& operator=(const V3Map& v3)
	{
		m_x = v3.m_x;m_y = v3.m_y;m_z = v3.m_z;
		return *this;
	}

	bool operator==(const V3Map& v3) const
	{
		return (fabs(m_x - v3.m_x) + fabs(m_y - v3.m_y) + fabs(m_z - v3.m_z)) < 0.05f;
	}

	bool operator !=(const V3Map& v3) const
	{
		return (fabs(m_x - v3.m_x) + fabs(m_y - v3.m_y) + fabs(m_z - v3.m_z)) >= 0.05f;
	}

	V3Map operator+(const V3Map& v3) const
	{
		return V3Map(m_x + v3.m_x, m_y + v3.m_y, m_z + v3.m_z);
	}

	V3Map operator-(const V3Map& v3) const
	{
		return V3Map(m_x - v3.m_x, m_y - v3.m_y, m_z - v3.m_z);
	}

	V3Map operator*(float a) const
	{
		return V3Map(m_x * a, m_y * a, m_z * a);
	}

	V3Map operator/(float a) const
	{
		if (fabs(a) < 1e-3) return;
		float oneOverA = 1.0f / a; 
		return V3Map(m_x * oneOverA, m_y * oneOverA, m_z * oneOverA);
	}

	V3Map& operator+=(const V3Map& v3)  
	{
		m_x += v3.m_x; m_y += v3.m_y; m_z += v3.m_z;
		return *this;
	}

	V3Map& operator-=(const V3Map& v3)
	{
		m_x -= v3.m_x; m_y -= v3.m_y; m_z -= v3.m_z;
		return *this;
	}

	V3Map& operator*=(float a)
	{
		m_x *= a; m_y *= a; m_z *= a;
		return *this;
	}

	V3Map& operator/=(float a)
	{
		if (fabs(a) < 1e-3) return;
		float oneOverA = 1.0f / a;
		m_x *= oneOverA; m_y *= oneOverA; m_z *= oneOverA;
		return *this;
	}

	void zero() { m_x = m_y = m_z = 0.0f; }
	// 正则化
	void normalize()
	{
		float magSq = m_x * m_x + m_y * m_y + m_z + m_z;
		if (magSq > 0.0f)
		{
			float oneOverMag = 1.0f / sqrt(magSq);
			m_x *= oneOverMag;
			m_y *= oneOverMag;
			m_z *= oneOverMag;
		}
	}

	// 向量点乘
	float operator*(const V3Map& v3)
	{
		return m_x * v3.m_x + m_y * v3.m_y + m_z * v3.m_z;
	}

};

// 向量取模
inline float VectorMag(const V3Map& v3)
{
	return sqrt(v3.m_x * v3.m_x + v3.m_y * v3.m_y + v3.m_z * v3.m_z);
}
// 向量点乘
inline float DotProduct(const V3Map& a, const V3Map& b)
{
	return a.m_x * b.m_x, a.m_y * b.m_y, a.m_z * b.m_z;
}
// 两个向量叉乘
inline V3Map CrossProduct(const V3Map& a, const V3Map& b)
{
	float x = a.m_y * b.m_z - a.m_z * b.m_y;
	float y = a.m_z * b.m_x - a.m_x * b.m_z;
	float z = a.m_x * b.m_y - a.m_y * b.m_x;
	return V3Map(x, y, z);
}
// 两点距离
inline float Distance(const V3Map& a, const V3Map& b)
{
	float dx = a.m_x - b.m_x;
	float dy = a.m_y - b.m_y;
	float dz = a.m_z - b.m_z;

	return sqrt(dx * dx + dy * dy + dz * dz);
}
// 计算弧度
inline float Radian(const V3Map& a, const V3Map& b)
{
	return acos(DotProduct(a , b)/(VectorMag(a)*VectorMag(b)));
}

// 计算两个向量之间夹角(角度转弧度: π/180×角度 ;弧度变角度:180/π×弧度)
inline float Angle(const V3Map& a, const V3Map& b)
{
	return Radian(a, b) * 180 / PI;
}