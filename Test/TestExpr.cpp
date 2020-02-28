#include "SFTest.h"
#include "expr/SimpleExpr.h"

void SFTest::TestExpr()
{
	const char* expr = "12 * 2/(10-8)";
	SimpleExpr oExpr;
	int ret = oExpr.CalcuExpr(expr);
	cout << "expr ret===" << ret << endl;
}