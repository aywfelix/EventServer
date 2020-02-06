
#include "TableMgr.h"

std::unique_ptr<TableMgr> g_pTableMgr = nullptr;

void TableMgr::LoadTables()
{
	if (!ActivityTable::Instance()->Load())
	{
		exit(-1);
	}
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