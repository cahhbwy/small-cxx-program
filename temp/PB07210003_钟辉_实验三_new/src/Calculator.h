#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <stack>
#include <queue>
#include "base.h"
using namespace std;
typedef int ElemType;

// 计算器类
template<class ElemType>
class Calculator{
private:
//  计算器的数据成员:
	queue<ElemType> exp;
	stack<ElemType> opnd;										// 操作数栈
	stack<ElemType> optr;										// 操作符栈


public:
// 计算器类方法声明:
	Calculator(	queue <ElemType> e ){
		exp = e;
	};											// 无参数的构造函数
	virtual ~Calculator(){};					// 析构函数
	string Run();									// 运算表达式
};


/*
 * ElemType:: isOpnd(), isOptr(), endOptr(), prior(), eval( opnd )
 */
template<class ElemType>
string Calculator<ElemType>::Run()
{
	optr.push(  ElemType::endOptr() );		// 在optr栈中加入一个结束符，哨兵

	while ( !optr.empty() ){
		if ( exp.front().isOpnd() ){
			opnd.push( exp.front() );			// 操作数入opnd栈
			exp.pop();					// 读入下一个字符
		}
		else if( exp.front().isOptr() ) {
			if( (optr.top().getName().compare("(") == 0) && (exp.front().getName().compare(")") == 0) ){
				// 去括号
				optr.pop();
				exp.pop();
			}
			else if ( ( optr.top().getName().compare("(") == 0 ) ||
					(optr.top().getPrior() < exp.front().getPrior()) ) {
				optr.push(exp.front());		// ch入optr栈
				exp.pop();				// 读入新字符
			}
			else {
				optr.top().eval( opnd );	// 运算结果入opnd栈
				optr.pop();
			}
		}
	}
	return opnd.top().getName();
};

#endif
