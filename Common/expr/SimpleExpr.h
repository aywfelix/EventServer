#pragma once

/*
	+  -  *  /  (   )  #
+   >  >  <	 <  <   >  >
-
/
(
)
#
*/
#include <stack>
#include <string>

class SimpleExpr
{
public:
	int CalcuExpr(const char* expr);
	char* GetNum(char* expr);
private:
	std::stack<char> m_stackOpr;
	std::stack<int> m_stackNum;
	char m_symbol[30];
};

