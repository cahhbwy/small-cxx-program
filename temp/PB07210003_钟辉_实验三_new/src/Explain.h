/*
 * Explain.h
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */

#ifndef EXPLAIN_H_
#define EXPLAIN_H_

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

#include "base.h"
#include "Tableinfo.h"
#include "Table.h"
#include "Print.h"
#include "ExpToken.h"

typedef enum{
	CREATE_CMD, LOAD_CMD, SHOW_CMD, DROP_CMD,
	INSERT_CMD, SELECT_CMD, DELETE_CMD, UPDATE_CMD,
	QUIT_CMD, EXIT_CMD, BAD_CMD
} CmdType;

class Explain {
	string cmd;
	int status;
	char need;
	size_t tmpStart, tmpCur;
	vector<string> strings;
	vector<string> token;
	Print printer;
public:
	Explain( bool showMsg = true ):printer(showMsg){};
	virtual ~Explain();

	void getCmd( istream &in );
	void explain( CmdType &type, string &table, void *&arg );
	void nextToken();
	string GetTableInfo( void *&info );
	string GetInsertInfo( void *&info );
	string GetSelectInfo( void *&info );
	string GetDeleteInfo( void *&info );
	string GetUpdateInfo( void *&info );

};

#endif /* EXPLAIN_H_ */
