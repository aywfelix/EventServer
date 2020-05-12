#pragma once

#include <string>
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourTileCache.h"
#include "DetourTileCacheBuilder.h"
#include "fastlz/fastlz.h"
#include "DetourCommon.h"

#include "common/Common.h"

// 此地图导航使用的recast-navigation 算法库实现(用于3D地图导航)

// load tile
static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

// load tile cache
static const int TILECACHESET_MAGIC = 'T' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'TSET';
static const int TILECACHESET_VERSION = 1;

struct TileCacheSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams meshParams;
	dtTileCacheParams cacheParams;
};

struct TileCacheTileHeader
{
	dtCompressedTileRef tileRef;
	int dataSize;
};

struct FastLZCompressor : public dtTileCacheCompressor
{
	virtual int maxCompressedSize(const int bufferSize)
	{
		return (int)(bufferSize * 1.05f);
	}

	virtual dtStatus compress(const unsigned char* buffer, const int bufferSize,
		unsigned char* compressed, const int /*maxCompressedSize*/, int* compressedSize)
	{
		*compressedSize = fastlz_compress((const void* const)buffer, bufferSize, compressed);
		return DT_SUCCESS;
	}

	virtual dtStatus decompress(const unsigned char* compressed, const int compressedSize,
		unsigned char* buffer, const int maxBufferSize, int* bufferSize)
	{
		*bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
		return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
	}
};

struct LinearAllocator : public dtTileCacheAlloc
{
	unsigned char* buffer;
	size_t capacity;
	size_t top;
	size_t high;

	LinearAllocator(const size_t cap) : buffer(0), capacity(0), top(0), high(0)
	{
		resize(cap);
	}

	~LinearAllocator()
	{
		dtFree(buffer);
	}

	void resize(const size_t cap)
	{
		if (buffer) dtFree(buffer);
		buffer = (unsigned char*)dtAlloc(cap, DT_ALLOC_PERM);
		capacity = cap;
	}

	virtual void reset()
	{
		high = dtMax(high, top);
		top = 0;
	}

	virtual void* alloc(const size_t size)
	{
		if (!buffer)
			return 0;
		if (top + size > capacity)
			return 0;
		unsigned char* mem = &buffer[top];
		top += size;
		return mem;
	}

	virtual void free(void* /*ptr*/)
	{
		// Empty
	}
};

// These are just sample areas to use consistent values across the samples.
/// The use should specify these base on his needs.
enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	SAMPLE_POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
	SAMPLE_POLYFLAGS_ALL = 0xffff	// All abilities.
};

struct MeshProcess : public dtTileCacheMeshProcess
{
	//InputGeom* m_geom;
	inline MeshProcess(){}
	//inline MeshProcess() : m_geom(0)
	//{
	//}

	//inline void init(InputGeom* geom)
	//{
	//	m_geom = geom;
	//}

	virtual void process(struct dtNavMeshCreateParams* params,
		unsigned char* polyAreas, unsigned short* polyFlags)
	{
		// Update poly flags from areas.
		for (int i = 0; i < params->polyCount; ++i)
		{
			if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
				polyAreas[i] = SAMPLE_POLYAREA_GROUND;

			if (polyAreas[i] == SAMPLE_POLYAREA_GROUND ||
				polyAreas[i] == SAMPLE_POLYAREA_GRASS ||
				polyAreas[i] == SAMPLE_POLYAREA_ROAD)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (polyAreas[i] == SAMPLE_POLYAREA_WATER)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (polyAreas[i] == SAMPLE_POLYAREA_DOOR)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}

		// Pass in off-mesh connections.
		//if (m_geom)
		//{
		//	params->offMeshConVerts = m_geom->getOffMeshConnectionVerts();
		//	params->offMeshConRad = m_geom->getOffMeshConnectionRads();
		//	params->offMeshConDir = m_geom->getOffMeshConnectionDirs();
		//	params->offMeshConAreas = m_geom->getOffMeshConnectionAreas();
		//	params->offMeshConFlags = m_geom->getOffMeshConnectionFlags();
		//	params->offMeshConUserID = m_geom->getOffMeshConnectionId();
		//	params->offMeshConCount = m_geom->getOffMeshConnectionCount();
		//}
	}
};

enum PathFindMode
{
	PATHFIND_FOLLOW = 1,  // 路径点非常细,小段距离点组成
	PATHFIND_STRAIGHT = 2, // 只记录拐点, 实用
	PATHFIND_SLICED = 3, // 只记录拐点, 只是在demo中加上了动画展示
	RAYCAST = 4 // 短距离寻路,直线,有拐点则失效
};

// reacast-navigation
class MapNav
{
public:
	MapNav();
	~MapNav();

	int FindPath(PathFindMode findmode, float* m_spos, float* m_epos, float* movepath);
	// 只用此函数寻路即可
	int FindPath(float* spos, float* epos, float* movepath);
	bool CanReach(const WorldPos& pos);
	// 添加障碍
	// Cylinder obstacle.
	bool AddObstacle(const WorldPos* pos, const float radius, const float height, dtObstacleRef* result);
	// Aabb obstacle.
	bool AddBoxObstacle(const WorldPos* pos, dtObstacleRef* result);
	bool RemoveObstacle(const dtObstacleRef* ref);

	bool LoadTile(const std::string& path);
	bool LoadCacheTile(const std::string& path);
	//// load map obj and gen tile file
	// 参考
	//bool LoadMesh(const std::string& path); --InputGeom::loadMesh(rcContext* ctx, const std::string& filepath)
	//bool buildMesh();  --Sample_SoloMesh::handleBuild()
private:
	bool LoadTile(const char* path);
	bool LoadCacheTile(const char* path); 
private:
	class dtNavMesh* m_navMesh;
	class dtNavMeshQuery* m_navQuery;
	class dtTileCache* m_tileCache;

	struct LinearAllocator* m_talloc;
	struct FastLZCompressor* m_tcomp;
	struct MeshProcess* m_tmproc;

	float m_polyPickExt[3];
	class dtQueryFilter m_filter;

	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;
};

