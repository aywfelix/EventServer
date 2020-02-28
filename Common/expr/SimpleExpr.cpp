#include "SimpleExpr.h"
#include "StringUtil.h"
#include "Util.h"

const char PRI[7][7] = {
	{'>','>','<','<','<','>','>'},
	{'>','>','<','<','<','>','>'},
	{'>','>','>','>','<','>','>'},
	{'>','>','>','>','<','>','>'},
	{'<','<','<','<','<','=','E'},
	{'>','>','>','>','E','>','>'},
	{'<','<','<','<','<','E','='}
};

const char* OprList = "+-*/()#";

char* SimpleExpr::GetNum(char* expr)
{
	char* tmp = expr;
	while (*tmp>='0' && *tmp <='9' || *tmp == '.')
	{
		tmp++;
	}
	float num;
	std::string strNum(expr, 0, tmp-expr);
	StrConvert(strNum, num);
	m_stackNum.push(num);
	return tmp;
}

float SimpleExpr::CalcuExpr(const char* expr)
{
	if (expr == nullptr) return -1;
	char iOpr;
	float num1, num2;
	bool do_end = false;
	char opr;
	char* pCur = const_cast<char*>(expr);
	m_stackOpr.push('#');
	while (!do_end)
	{
		while (pCur && *pCur == ' ')
		{
			pCur++;
		}
		if (pCur && *pCur != '\0')
		{
			opr = *pCur;
		}
		else
		{
			opr = '#';
			do_end = true;
		}
		const char* pOpr = strchr(OprList, opr);
		if (pOpr == nullptr) // 如果是数字
		{
			pCur = GetNum(pCur);
			continue;
		}
		int i = strchr(OprList, m_stackOpr.top()) - OprList;
		int j = pOpr - OprList;
		switch (PRI[i][j])
		{
		case '<':
			m_stackOpr.push(pOpr[0]);
			pCur++;
			break;
		case '=':
			m_stackOpr.pop();
			pCur++;
			break;
		case '>':
			iOpr = m_stackOpr.top();
			m_stackOpr.pop();
			num1 = m_stackNum.top();
			m_stackNum.pop();
			num2 = m_stackNum.top();
			m_stackNum.pop();
			switch (iOpr)
			{
			case '+':num2 += num1; break;
			case '-':num2 -= num1; break;
			case '*':num2 *= num1; break;
			case '/':num2 /= num1; break;
			}
			m_stackNum.push(num2);
			break;
		case 'E':
			return -1;
		}
		
	}
	return m_stackNum.top();
}

// CalcuExpr("12 * 2/(10-8)")