/*
 * Table.h
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <vector>
#include <algorithm>
#include "base.h"
#include "check.h"
#include "tableinfo.h"

class Table {

	char	name[NAME_MAX_LEN];
	char	tmpStr[TMP_STR_SIZE];

	void InitPos();

public:
	char	* buf;
	TableInfo info;

	Table( const char *newName = "" );
	Table( const string newName );
	Table( const Table &r );
	Table &operator = ( const Table &r );
	virtual ~Table();
	void Rename( const char * newName );
	char *FileName();
	void Read();
	void Write();
	bool Load();
	bool Drop();
	bool Create( const TableInfo &i );
	bool Insert( vector<string> item );
	void Select( vector<int> &selected, const vector<string> &check );
	void Select( vector<int> &selected  );
	bool Delete( vector<int> lines );
	bool Update( vector<int> lines, vector<int> cols, vector<string> value  );
	void ColNum( vector<int> &cols  );
	void ColNum( vector<int> &coln, const vector<string> &cols );
	TableInfo Show() const;
	bool NoRef( int colNum, string elem );
	bool IsRef( int colNum, string elem );

};

#endif /* TABLE_H_ */
