/*
 * ExpToken.h
 *
 *  Created on: 2010-6-6
 *      Author: Hui
 */

#ifndef EXPTOKEN_H_
#define EXPTOKEN_H_

#include <sstream>
#include <string>
#include <stack>
using namespace std;
#include "base.h"

extern const char *g_Optr[OPTR_MAX_NUM];


class ExpToken{
	string name;
	int optrNum;

	double getNum();
public:
	static ExpToken endOptr();
	~ExpToken();
	ExpToken( string  e );
	void eval( stack<ExpToken> &opnd );
	bool isOpnd();
	bool isOptr();
	int getPrior();
	string getName();
};

#endif /* EXPTOKEN_H_ */
