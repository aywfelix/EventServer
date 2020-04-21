#include "SFTest.h"
#include "tbpb/SqlUtil.h"

#include <vector>
#include <map>

void SFTest::TestSql()
{
	vector<string> fields, tbls;
	map<string, string> condition;
	fields.emplace_back("name");
	fields.emplace_back("age");
	tbls.emplace_back("person");
	string sql = SqlUtil::Select(fields, tbls, condition);
	std::cout << sql << std::endl;
}