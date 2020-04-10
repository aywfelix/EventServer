#pragma once
#include "MemPool.hpp"

class Obj;
using obj_pool_t = MemPool<Obj>;
class ObjPool
{
public:
	ObjPool();
	~ObjPool();
	Obj* NewObj();
	Obj* GetObj(int32_t memid);
	void DelObj(Obj* player);
private:
	obj_pool_t m_obj_pool;
};

extern std::unique_ptr<ObjPool> g_pObjPool;
