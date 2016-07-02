/*
 * TableInfo.cpp
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */

#include "TableInfo.h"


TableInfo::TableInfo( ColInfo *newCol, int newColNum, int newLineNum ){
	col = newCol;
	colNum = newColNum;
	lineNum = newLineNum;

	UpdateIndex();
}

TableInfo::~TableInfo() {
	if( col ){
		delete( col );
	}
}

TableInfo::TableInfo( const TableInfo &r ) {
	lineNum = r.lineNum;
	colNum = r.colNum;
	colWidth = r.colWidth;
	col = new ColInfo[colNum];
	for( int i=0; i<colNum; i++ ){
		col[i] = r.col[i];
	}
}

TableInfo & TableInfo::operator = ( const TableInfo& r ) {
	lineNum = r.lineNum;
	colNum = r.colNum;
	colWidth = r.colWidth;
	checker = r.checker;
	col = new ColInfo[colNum];
	for( int i=0; i<colNum; i++ ){
		col[i] = r.col[i];
	}
	return *this;
}

void TableInfo::Read( FILE *f ){
	fread( &lineNum, sizeof(lineNum), 1,  f );
	fread( &colNum, sizeof(colNum), 1,  f );
	fread( &colWidth, sizeof(colWidth), 1,  f );
	col = (ColInfo *)malloc( colNum * sizeof( ColInfo) );
	fread( col, colNum, sizeof( ColInfo),  f);

	checker.read( f );
}

void TableInfo::Write( FILE *f ) const{
	fwrite( &lineNum, sizeof(lineNum), 1,  f );
	fwrite( &colNum, sizeof(colNum), 1,  f );
	fwrite( &colWidth, sizeof(colWidth), 1,  f );
	fwrite( col, colNum, sizeof( ColInfo),  f);

	checker.write( f );
}

/*
 * update the index information based on column length.
 */
void TableInfo::UpdateIndex(){

	size_t index =0;
	for( int i=0; i<colNum; i++ ){
		col[i].index = index;
		index += col[i].length;
	}
	colWidth = index;
}
