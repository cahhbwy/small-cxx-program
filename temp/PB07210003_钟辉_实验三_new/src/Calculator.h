#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <stack>
#include <queue>
#include "base.h"
using namespace std;
typedef int ElemType;

// ��������
template<class ElemType>
class Calculator{
private:
//  �����������ݳ�Ա:
	queue<ElemType> exp;
	stack<ElemType> opnd;										// ������ջ
	stack<ElemType> optr;										// ������ջ


public:
// �������෽������:
	Calculator(	queue <ElemType> e ){
		exp = e;
	};											// �޲����Ĺ��캯��
	virtual ~Calculator(){};					// ��������
	string Run();									// ������ʽ
};


/*
 * ElemType:: isOpnd(), isOptr(), endOptr(), prior(), eval( opnd )
 */
template<class ElemType>
string Calculator<ElemType>::Run()
{
	optr.push(  ElemType::endOptr() );		// ��optrջ�м���һ�����������ڱ�

	while ( !optr.empty() ){
		if ( exp.front().isOpnd() ){
			opnd.push( exp.front() );			// ��������opndջ
			exp.pop();					// ������һ���ַ�
		}
		else if( exp.front().isOptr() ) {
			if( (optr.top().getName().compare("(") == 0) && (exp.front().getName().compare(")") == 0) ){
				// ȥ����
				optr.pop();
				exp.pop();
			}
			else if ( ( optr.top().getName().compare("(") == 0 ) ||
					(optr.top().getPrior() < exp.front().getPrior()) ) {
				optr.push(exp.front());		// ch��optrջ
				exp.pop();				// �������ַ�
			}
			else {
				optr.top().eval( opnd );	// ��������opndջ
				optr.pop();
			}
		}
	}
	return opnd.top().getName();
};

#endif
