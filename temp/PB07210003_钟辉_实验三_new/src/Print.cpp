/*
 * Print.cpp
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */
#include "Print.h"

Print::Print( bool showMsg ) {
	show = showMsg;
}

Print::~Print() {
	// TODO Auto-generated destructor stub
}
void Print::printLine() const{
	cout << " ";
	for( int i=0; i<colNum; i++ ){
		cout << "--------";
	}
	cout << endl;
}
void Print::printHeader() const{
	printLine();
}
void Print::printFooter() const{
	printLine();
}
void Print::printLineStart() const{
	cout << "| ";
}
void Print::printLineEnd() const{
	cout << " |" << endl;
}

void Print::print( TableInfo t ){
	if( !show ){
		return;
	}
	colNum = 5;
	printHeader();
	printLineStart();
	cout << "col" << '\t' << "length" << "\t" << "key" << "\t" << "integer" << "\t" << "notnull" << "\t";
	printLineEnd();
	printLine();
	for( int line=0; line< t.colNum; line++ ){
		printLineStart();
		cout << t.col[line].name << '\t' << t.col[line].length << "\t";
		if( t.col[line].isKey ){
			cout << "key";
		}
		cout << "\t";
		if( t.col[line].isInteger ){
			cout << "integer";
		}
		cout << "\t";
		if( t.col[line].isNotNull ){
			cout << "notnull";
		}
		cout << "\t";
		printLineEnd();
	}
	printFooter();
}

void Print::print(){
	print( lastTable, lastLines, lastCols );
}
void Print::print( Table &table, vector<int> selected, vector<int> cols ){
	lastTable = table;
	lastLines = selected;
	lastCols = cols;

	if( !show ){
		return;
	}

	colNum = cols.size();

	printHeader();
	printLineStart();
	for( unsigned int i=0; i< cols.size(); i++ ){
		cout << table.info.col[cols[i]].name << '\t';
	}
	printLineEnd();
	printLine();
	string item;
	int post;
	for( unsigned int line = 0; line<selected.size(); line++ ){
		printLineStart();
		for( unsigned int i=0; i< cols.size(); i++ ){
			post = selected[line]*table.info.colWidth + table.info.col[cols[i]].index;
			cout << table.buf+post << '\t';
		}
		printLineEnd();
	}
	printFooter();
}

void Print::println( string s ) const{
	if( !show ){
		return;
	}
	cout << s << endl;
}
void Print::print( string s ) const{
	if( !show ){
		return;
	}
	cout << s;
}
void Print::test( bool err ) const{
	if( err ){
		println( "You shouldn't see me. There are something wrong. *_*\n" );
	}
}

bool Print::isAvail(){
	return show;
}
void Print::turnOn( bool showMsg ){
	show = showMsg;
}

Print & Print::operator << ( string msg ){
	print( msg );
	return *this;
}

Print err(true), warning(true);
