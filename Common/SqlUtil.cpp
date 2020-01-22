#include "SqlUtil.h"

string SqlUtil::Select(const string& fields, const string& tbls, map<string, string>& condition, bool isdesc = false)
{
	string sql = "";
	sql = "SELECT " + fields + " FROM " + tbls;
	if (!condition.empty())
	{
		sql += " WHERE " + JoinMap(condition);
	}
	if (isdesc)
	{
		sql += " ORDER BY DESC;";
	}
	return sql;
}

string SqlUtil::Select(vector<string>& fields, vector<string>& tbls, map<string, string>& condition, bool isdesc)
{
	string sql = "";
	sql = "SELECT " + JoinVec(fields) + " FROM " + JoinVec(tbls);
	if (!condition.empty())
	{
		sql += " WHERE " + JoinMap(condition);
	}
	if (isdesc)
	{
		sql += " ORDER BY DESC;";
	}
	return sql;
}

string SqlUtil::Insert(const string& tbl, vector<string>& fields, vector<string>& values)
{
	string sql = "";
	if (fields.empty()) { return InsertInto(tbl, values); }
	sql = "INSERT " + tbl + "(" + JoinVec(fields) + ") VALUES (" + JoinVec(values) + ");";
	return sql;
}

string SqlUtil::InsertInto(const string& tbl, vector<string>& values)
{
	string sql = "";
	sql = "INSERT INTO " + tbl + " VALUES (" + JoinVec(values) + ");";
	return sql;
}

string SqlUtil::Update(const string& tbl, map<string, string>& setting, map<string, string>& condition)
{
	string sql = "";
	sql = "UPDATE " + tbl + " SET " + JoinMap(setting) + " WHERE " + JoinMap(condition) + ";";
	return sql;
}

string SqlUtil::Delete(const string& tbl, map<string, string>& condition)
{
	string sql = "";
	sql = "DELETE FROM " + tbl + +" WHERE " + JoinMap(condition) + ";";
	return sql;
}

string SqlUtil::JoinVec(vector<string>& vec)
{
	if (vec.empty()) return "*";
	string tmp = "";
	for (auto& it : vec)
	{
		if (tmp == "") 
		{
			tmp = it;
			continue;
		}
		tmp += "," + it;
	}
	return tmp;
}

string SqlUtil::JoinMap(map<string, string>& mp)
{
	string tmp = "";
	for (auto& it : mp)
	{
		if (tmp == "")
		{
			tmp = it.first;
			tmp += "=" + it.second;
		}
		tmp += "," + it.first;
		tmp += "=" + it.second;
	}
	return tmp;
}