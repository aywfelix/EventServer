#pragma once
#include <string>
#include <map>
#include <vector>

using std::string;
using std::vector;
using std::map;

class SqlUtil
{
public:
	static string Select(const string& fields, const string& tbls, map<string, string>& condition, bool isdesc = false);
	static string Select(vector<string>& fields, vector<string>& tbls, map<string, string>& condition, bool isdesc=false);
	static string Insert(const string& tbl, vector<string>& fields, vector<string>& values);
	static string InsertInto(const string& tbl, vector<string>& values);
	static string Update(const string& tbl, map<string, string>& setting, map<string, string>& condition);
	static string Delete(const string& tbl, map<string, string>& condition);
private:
	static string JoinVec(vector<string>& vec);
	static string JoinMap(map<string, string>& mp);
};

