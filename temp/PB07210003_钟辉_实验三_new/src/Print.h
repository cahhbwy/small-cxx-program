/*
 * Print.h
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */

#ifndef PRINT_H_
#define PRINT_H_

#include "tableinfo.h"
#include "table.h"

#include <iostream>
#include <vector>
using namespace std;

class Print {
	int colNum;
	bool show;
	void printHeader() const;
	void printFooter() const;
	void printLine() const;
	void printLineStart() const;
	void printLineEnd() const;
public:
	Table lastTable;
	vector<int> lastLines;
	vector<int> lastCols;
	Print( bool showMsg = true );
	virtual ~Print();
	void print();
	void print( TableInfo t );
	void print( Table &table, vector<int> selected, vector<int> cols );
	void println( string s ) const;
	void print( string s ) const;
	Print & operator << ( string msg );
	void test( bool err=true ) const;
	bool isAvail();
	void turnOn( bool showMsg = true );

};

extern Print err, warning;

#endif /* PRINT_H_ */
