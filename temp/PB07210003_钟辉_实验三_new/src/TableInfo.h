/*
 * TableInfo.h
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */

#ifndef TABLEINFO_H_
#define TABLEINFO_H_

#include <malloc.h>
#include <cstdio>
#include <cstring>
#include "base.h"

#include "colinfo.h"
#include "check.h"

class TableInfo {
public:
	int 	lineNum;
	int 	colNum;
	size_t	colWidth;
	ColInfo	*col;
	Check	checker;

	TableInfo( ColInfo *newCol = NULL, int newColNum = 0, int newLineNum = 0 );
	virtual ~TableInfo();

	void Read( FILE *f );
	void Write( FILE *f ) const;
	void UpdateIndex();
	TableInfo & operator = ( const TableInfo& r );
	TableInfo( const TableInfo &r );
};

#endif /* TABLEINFO_H_ */
