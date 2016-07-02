/*
 * Core.cpp
 *
 *  Created on: 2010-6-5
 *      Author: Hui
 */

#include "Core.h"


Core::~Core(){
	// TODO Auto-generated destructor stub
}

void Core::Run(){
	CmdType type;
	void *arg;
	string table;
	string cmd;
	Table *t;

start:
	arg = NULL;
	e.getCmd( in );
	e.explain( type, table, arg );

	t = new Table( table );
	t->Rename( table.c_str() );

	switch( type ){
		case CREATE_CMD: {
			if( t->Create( *(TableInfo*)arg ) ){
				printer.println("Okay ^_^");
			}
			break;
		}
		case LOAD_CMD: {
			if( t->Load() ){
				printer.println("Okay -_-");
			}
			break;
		}
		case DROP_CMD: {
			if( t->Drop() ){
				printer.println("Okay *_*");
			}
			break;
		}
		case SHOW_CMD: {
			t->Read();
			printer.print( t->Show() );
			break;
		}
		case INSERT_CMD: {
			t->Read();
			if( t->Insert( * (vector<string> *) arg ) ){
				printer.println("Okay :P");
			}
			break;
		}
		case SELECT_CMD: {
			vector<string>* *info = (vector<string>* *)arg;
			vector<string> &colStr = *info[0];
			vector<string> &checkStr = *info[1];

			t->Read();

			vector<int> colNum;
			if( colStr.size() == 1 && colStr[0].compare("*")==0 ){
				t->ColNum( colNum );
			}
			else{
				t->ColNum( colNum, colStr );
			}

			vector<int> selected;
			if( checkStr.size() == 0 ){
				t->Select( selected );
			}
			else{
				t->Select( selected, checkStr );
			}
			printer.print(*t, selected, colNum );
			break;
		}
		case DELETE_CMD: {
			vector<string> &checkStr = *(vector<string>*)arg;
			vector<int> selected;
			t->Select( selected, checkStr );
			if( t->Delete( selected ) ){
				printer.println("Okay @_@");
			}
			break;
		}
		case UPDATE_CMD: {
			vector<string>* *info = (vector<string>* *)arg;
			vector<string> &colStr = *info[0];
			vector<string> &value = *info[1];
			vector<string> &checkStr = *info[2];

			t->Read();
			vector<int> colNum;
			t->ColNum( colNum, colStr );
			vector<int> lineNum;
			t->Select( lineNum, checkStr );
			if( t->Update( lineNum, colNum, value ) ){
				printer.println("Okay #_#");
			}
			break;
		}
		case QUIT_CMD:
			return;
		case EXIT_CMD:
			exit(0);
		case BAD_CMD:
			printer.println( "Bad Command");
			printer.println( "Only support create, load, drop, show, insert, select, delete, quit, exit" );
			break;
		default: {
			printer.test();
		}
	}
	delete t;

	goto start;
}
