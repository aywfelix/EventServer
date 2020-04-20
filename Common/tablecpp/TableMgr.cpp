#include "TableMgr.h"
#include "Activity.hpp"
#include "Scene.hpp"

std::unique_ptr<TableMgr> g_pTableMgr = nullptr;

void TableMgr::LoadTables()
{
	ActivityTable::Instance()->Load();
	SceneTable::Instance()->Load();
}

bool TableMgr::ReLoadTables()
{
	if (!ActivityTable::Instance()->ReLoad())
	{
		return false;
	}
	return true;
}

bool TableMgr::CheckTables()
{
	return true;
}