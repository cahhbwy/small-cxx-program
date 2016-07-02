/*
 * Check.cpp
 *
 *  Created on: 2010-6-5
 *      Author: Hui
 */

#include "check.h"

Check::Check() {
}
//Check::Check( const Check &r ){
//	*this = r;
//}
//Check & Check::operator = ( const Check &r ){
//	this->exp.clear();
//	for( unsigned int i=0; i< r.exp.size(); i++ ){
//		this->exp.push_back(r.exp[i]);
//	}
//	return *this;
//}

Check::~Check() {
	// TODO Auto-generated destructor stub
}

Check::Check( const vector<string> &condition, ColInfo *cols, int colNum ){
	set( condition, cols, colNum );
}
void Check::set( const vector<string> &condition, ColInfo *cols, int colNum ){
	exp = condition;

	// replace column name with $column_position
	for( unsigned int i=0; i< exp.size(); i++ ){
		int j;
		for( j=0; j< colNum; j++ ){
			if( exp[i].compare( cols[j].name ) == 0 ){
				vector<int> e;
				exp[i].assign("$");
				char tmpStr[10];
				sprintf(tmpStr,"%d",cols[j].index);
				exp[i].append( tmpStr );
				break;
			}
		}
	}
}



bool Check::check( char *line ) const{


	queue<ExpToken> q;
//	cout << endl;
	for( unsigned int i=0; i<exp.size(); i++ ){
		if( exp[i].at(0) == '$'){
			int index = atoi( exp[i].substr(1).c_str() );
			ExpToken t( line + index );
			q.push( t );
//			cout << t.getName() << '\t';
		}
		else{
			ExpToken t( exp[i] );
			q.push(t);
//			cout << t.getName() << '\t';
		}
	}
//	cout << endl;
	ExpToken expEnd("#");
	q.push( expEnd );
	Calculator <ExpToken> cal(q);
	return cal.Run().compare( "0" ) != 0;
}

void Check::read( FILE *f ){
	int expSize = 0;
	fread( &expSize, sizeof(expSize), 1, f );
	if( expSize == 0 ){
		return;
	}
	int stringSize = 0;
	char *buf;
	string curExpElem;
	exp.clear();
	for( int i=0; i<expSize; i++ ){
		stringSize = 0;
		fread( &stringSize, sizeof( stringSize ), 1, f  );
		buf = new char[stringSize];
		fread( buf, stringSize, sizeof(char), f );
		curExpElem.assign(buf);
		exp.push_back(curExpElem);
		delete buf;
	}
}
void Check::write( FILE *f ) const {
	int expSize = exp.size();
	fwrite( &expSize, sizeof(expSize), 1, f );
	if( expSize == 0 ){
		return;
	}
	int stringSize;
	for( int i=0; i<expSize; i++ ){
		stringSize = exp[i].size()+1;
		fwrite( &stringSize, sizeof( stringSize ), 1, f  );
		fwrite( exp[i].c_str(), stringSize, sizeof(char), f );
	}
}

