#include "ObjPool.h"
#include "Obj.h"

ObjPool::ObjPool() {}
ObjPool::~ObjPool() {}

Obj* ObjPool::NewObj()
{
	Obj* obj = m_obj_pool.NewElem();
	if (obj == nullptr) return nullptr;
	return obj;
}

Obj* ObjPool::GetObj(int32_t memid)
{
	Obj* obj = m_obj_pool.GetElem(memid);
	if (obj == nullptr) return nullptr;
	return obj;
}

void ObjPool::DelObj(Obj* obj)
{
	if (obj == nullptr) return;
	m_obj_pool.DelElem(obj->GetMemId());
}