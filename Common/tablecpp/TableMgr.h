#pragma once
#include <memory>

class TableMgr
{
public:
	void LoadTables();
	bool ReLoadTables();
	bool CheckTables();
private:
};

extern std::unique_ptr<TableMgr> g_pTableMgr;