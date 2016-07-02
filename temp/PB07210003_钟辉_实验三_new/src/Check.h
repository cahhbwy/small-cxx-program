/*
 * Check.h
 *
 *  Created on: 2010-6-5
 *      Author: Hui
 */

#ifndef CHECK_H_
#define CHECK_H_

#include <cstring>
#include <vector>
#include <stack>
#include <queue>
#include <cstdio>

#include "base.h"
#include "colinfo.h"
#include "ExpToken.h"
#include "Calculator.h"

class Check {
	vector<string> exp;
public:
//	Check( const Check &r );
//	Check & operator = ( const Check &r );
	Check();
	Check( const vector<string> &condition, ColInfo *cols, int colNum );
	virtual ~Check();
	void set( const vector<string> &condition, ColInfo *cols, int colNum );
	bool check( char *e) const;
	void read( FILE *f );
	void write( FILE *f ) const;
};

#endif /* CHECK_H_ */
