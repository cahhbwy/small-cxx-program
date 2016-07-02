/*
 * Explain.cpp
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */

#include "Explain.h"
#include "Core.h"

Explain::~Explain() {

}

void Replace( string &str, string searchString, string replaceString ){
    if( searchString == replaceString ){
    	return;
    }
    string::size_type pos = 0;
    while ( (pos = str.find(searchString, pos)) != string::npos ) {
        str.replace( pos, searchString.size(), replaceString );
        pos += replaceString.size();
    }
}

void Divide( string &str, string optr ){
	string replaceString = " " + optr + " ";
	Replace( str, optr, replaceString );
}

void Explain::nextToken(){

	switch( status ){
	case 0:
		if( cmd.at(tmpCur) == '\''){
			status = 1;
			tmpStart = tmpCur;
		}
		else if( cmd.at(tmpCur) == ';'){
			status = 3;
		}
		break;
	case 1:
		if( cmd.at(tmpCur) == '\\' ){
			status = 2;
			cmd.erase(cmd.begin()+tmpCur);
			tmpCur--;
		}
		else if( cmd.at(tmpCur) == '\'' ){	// replace string with mark 'vector_index
			status = 0;
			char strPos[10];
			sprintf(strPos,"%lu",strings.size());
			string strMark(" '");
			strMark.append( strPos );
			strMark.append(" ");
			strings.push_back( cmd.substr(tmpStart+1, tmpCur-tmpStart-1 ) );
			cmd.replace( tmpStart, tmpCur - tmpStart+1, strMark );
			tmpStart = tmpStart + strMark.size()-1;
			tmpCur = tmpStart;
		}
		break;
	case 2:
		status = 1;
		if( cmd.at(tmpCur) == 't' ){
			cmd.replace( tmpCur, 1, "\t" );
		}
		else if( cmd.at(tmpCur) == 'n' ){
			cmd.replace( tmpCur, 1, "\n" );
		}
		break;
	case 3:
		// TODO
		printer.println( "Warning. find char after ';'. Ignore" );
		cmd.erase( cmd.begin()+tmpCur, cmd.end());
		status = 3;
		break;
	}
	tmpCur++;
}

void Explain::getCmd( istream &in ){

	char cmdBuf[CMD_MAX_LEN];
	printer.print( "BenSQL>" );
	token.clear();
	cmd.clear();
	tmpStart = 0;
	tmpCur = 0;
	status = 0;
	while(1){
		if( in.getline( cmdBuf, CMD_MAX_LEN -1 ) == 0 ){
			// TODO exit
			exit(0);
		}
		printer.println( cmdBuf );
		cmd.append( cmdBuf );
//		cout << cmd << endl;
		while(tmpCur < cmd.size() ){
			nextToken();
//			cout << status;
		}
//		cout << endl << cmd << endl;
		if( status == 0 ){
			printer.print("     ;>");
		}
		else if( status <= 2 ){
			printer.print("     \'>");
		}
		else if( status == 3 ){
			break;
		}
	}
/*
	cout << "cmd:" << cmd << endl;
	for( int i=0; i<strings.size(); i++ ){
		cout << i << ": " << strings[i] << endl;
	}
*/
	cmd.erase( cmd.end()-1 );	// delete ';' in the end of statement.
	for( int i=0; g_Optr[i][0]; i++ ){
		Divide( cmd, g_Optr[i] );
	}
	istringstream instr(cmd);
	string buf;
	while( instr>>buf ){
		if( buf.at(0) == '\''){
			token.push_back( strings[atoi( buf.c_str() + 1 )] );
		}
		else{
			token.push_back( buf );
		}
	}
}

void Explain::explain( CmdType &type, string &table, void *&arg ){
	if( token[0].compare("create") == 0 ){
		type = CREATE_CMD;
		table = GetTableInfo( arg );
	}
	else if( token[0].compare("load") == 0 ){
		type = LOAD_CMD;
		table = token[2];
	}
	else if( token[0].compare("drop") == 0 ){
		type = DROP_CMD;
		table = token[2];
	}
	else if( token[0].compare("show") == 0 ){
		type = SHOW_CMD;
		table = token[2];
	}
	else if( token[0].compare("insert") == 0 ){
		type = INSERT_CMD;
		table = GetInsertInfo( arg );
	}
	else if( token[0].compare("select") == 0 ){
		type = SELECT_CMD;
		table = GetSelectInfo( arg );
	}
	else if( token[0].compare("delete") == 0 ){
		type = DELETE_CMD;
		table = GetDeleteInfo( arg );
	}
	else if( token[0].compare("update") == 0 ){
		type = UPDATE_CMD;
		table = GetUpdateInfo( arg );
	}
	else if( token[0].compare("quit") == 0 ){
		type = QUIT_CMD;
	}
	else if( token[0].compare("exit") == 0 ){
		type = EXIT_CMD;
	}
	else{
		type = BAD_CMD;
	}
}

//example: update t set name = hui  where id = 1
string Explain::GetUpdateInfo( void *&arg ){
	string table;
	unsigned int i=3;
	vector<string> *cols = new vector<string>;
	vector<string> *sets = new vector<string>;
	while(1){
		cols->push_back(token[i]);
		sets->push_back(token[i+2]);
		if( token[i+3].compare("where") == 0 ){
			break;
		}
		else if( token[i+3].compare(",") ){
			// TODO throw error code;
			break;
		}
		i+=4;
	}
	i+=4;
	vector<string> *where = new vector<string>;
	while( i< token.size() ){
		where->push_back(token[i]);
		i++;
	}
	vector<string>* *a = new vector<string> *[3];
	a[0] = cols;
	a[1] = sets;
	a[2] = where;
	arg = (void*)a;

	return token[1];

}


//example: delete from t where id = 1
string Explain::GetDeleteInfo( void *&arg ){
	string table;
	unsigned int i=4;
	vector<string> *where = new vector<string>;
	while( i< token.size() ){
		where->push_back(token[i]);
		i++;
	}
	arg = (void*)where;

	return token[2];

}



//example: select * from t where id = 1
// select name , type from t where id > 2 and time <= 10
string Explain::GetSelectInfo( void *&arg ){
	string table;
	unsigned int i=1;
	vector<string> *cols = new vector<string>;
	while( token[i].compare("from") ){
		if( token[i].compare(",")){
			cols->push_back(token[i]);
		}
		i++;
	}
	table = token[i+1];

	while( i < token.size() ){
		if( token[i++].compare("where") == 0 ){
			break;
		}
	}
	vector<string> *where = new vector<string>;
	while( i< token.size() ){
		where->push_back(token[i]);
		i++;
	}
	vector<string>* *a = new vector<string> *[2];
	a[0] = cols;
	a[1] = where;
	arg = (void*)a;

	return table;

}


//example: insert into t value $LB 11 $COMMA a $COMMA 21 $RB
string Explain::GetInsertInfo( void *&ee ){
	vector<string> *e = new vector<string>;
	string last;
	for( unsigned int i=5; i< token.size(); i++ ){
		if( token[i].compare(",")==0 ){
//			cout << "{" << last << "} ";
			e->push_back( last );
			last.clear();
		}
		else if( token[i].compare(")")==0 ){
			break;
		}
		else{
			last = token[i];
		}
	}
	e->push_back( last );
	ee = (void*)e;
	return token[2];
}

//example: create table t $LB id 4 int key $COMMA s 3 int $COMMA s2 5  notnull $RB
string Explain::GetTableInfo( void *&tt ){

	bool oldErrStatus = err.isAvail();
	err.turnOn(false);

	TableInfo *t = new TableInfo;
	t->colNum = 1;
	for( unsigned int i=4; i < token.size(); i++ ){
		if( token[i].compare(",") == 0 ){
			t->colNum++;
		}
		else if( token[i].compare(")") == 0 ){
			break;
		}
	}
	t->col = new ColInfo[t->colNum];

//	cout << endl << t->colNum << endl;

	unsigned int i=4;
	for( int j=0; j< t->colNum; j++ ){

//		cout << i << " " << j << endl;

		t->col[j].isInteger = false;
		t->col[j].isNotNull = false;
		t->col[j].isKey = false;


		strcpy( t->col[j].name, token[i++].c_str() );
		t->col[j].length = atoi( token[i++].c_str() );
		while( token[i].compare(",") && token[i].compare(")") && i<token.size() ){
			if( token[i].compare("int")==0){
				t->col[j].isInteger = true;
			}
			if( token[i].compare("notnull")==0){
				t->col[j].isNotNull = true;
			}
			if( token[i].compare("key")==0){
				t->col[j].isKey = true;
			}
			if( token[i].compare("ref")==0){
				string insRef = "insert into ref value( ";
				insRef += token[2];
				insRef += ", ";
				insRef += t->col[j].name;
				insRef += ", ";
				insRef += token[++i];
				insRef += ", ";
				insRef += token[++i];
				insRef += ");\nquit;\n";
				std::istringstream ss(insRef);
				Core ins(ss, false);
				ins.Run();
			}
			i++;
		}
		i++;
	}
	t->UpdateIndex();

	while( i < token.size() ){
		if( token[i++].compare("where") == 0 ){
			break;
		}
	}
	vector<string> *where = new vector<string>;
	if( i >= token.size() ){
		where->push_back( "1");
	}
	else{
		while( i< token.size() ){
			where->push_back(token[i]);
			i++;
		}
	}

	t->checker.set( *where, t->col, t->colNum );
	delete where;

	tt = (void*)t;

	err.turnOn(oldErrStatus );

	return token[2];
}

