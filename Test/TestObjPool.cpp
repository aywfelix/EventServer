#include "TestObjPool.h"
#include "SFTest.h"


void SFTest::TestObjPool()
{
	auto p = ObjPoolFixed<user>::Instance();

	p->Init("xxxxxxxxxxxx");
	auto& obj = p->Get();
	cout << obj->m_str<<" : "<< obj->m_id << endl;

	obj = nullptr;
	obj = nullptr;
	//auto pool = ObjPool<user>::Instance();
	//auto p = pool->Get();
	//p->print("aaaaaaaaa");
}