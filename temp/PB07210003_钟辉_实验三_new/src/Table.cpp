/*
 * Table.cpp
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */
#include "Table.h"

Table::Table( const char * newName ) {
	Rename( newName );
}

Table::Table( const string newName ) {
	Table( newName.c_str() );
}

Table::Table( const Table &r ) {
	*this = r;
}

Table &Table::operator = ( const Table &r ) {
	strcpy( name, r.name );
	strcpy( tmpStr, r.tmpStr );
	info = r.info;
	int bufSize = info.lineNum * info.colWidth;
	char * buf = r.buf;
	if( buf ){
		buf = new char[bufSize];
		for( int i=0; i< bufSize; i++ ){
			buf[i] = r.buf[i];
		}
	}
	return *this;
}

Table::~Table() {
	if( buf ){
		delete buf;
	}
}

void Table::Rename( const char * newName ) {
	strncpy( name, newName, NAME_MAX_LEN-1 );
	buf = NULL;
}

char * Table::FileName(){
	strcpy( tmpStr, name );
	strcat( tmpStr, ".ben" );
	return tmpStr;
}

#include "core.h"
bool Table::Load(){
	string sql = "select * from BenSQL where name = ";
	sql += name;
	sql += ";\n";
	sql += "quit;\n";
	std::istringstream ss(sql);
	Core test(ss,false);
	test.Run();
	if( test.printer.lastLines.size() > 0 ){
		err << "Sorry, drop previous table first.\n";
		return false;
	}
	sql = "insert into BenSQL values(";
	sql += name;
	sql += "); \n";
	sql += "quit;\n";
	std::istringstream loadSS(sql);
	Core load(loadSS, false);
	return true;
}

bool Table::Drop(){
	string testRefQuery = "select from_table, from_col from ref where to_table =";
	testRefQuery += name;
	testRefQuery += ";\nquit;\n";
	std::istringstream testRefSS(testRefQuery);
	Core testRefCore(testRefSS,false);
	testRefCore.Run();
	if( testRefCore.printer.lastLines.size() > 0 ){
		err << "Sorry, table " << name << " is refered.\n";
		return false;
	}

	string sql = "delete from BenSQL where name = ";
	sql += name;
	sql += ";\n";
	sql += "delete from ref where from_table = ";
	sql += name;
	sql += ";\n";
	sql += "quit;\n";
	std::istringstream ss(sql);
	Core drop(ss,false);
	drop.Run();
	return true;
}

bool Table::Create( const TableInfo &i ){
	info = i;
	buf = NULL;
	Write();
//	cout << "load start" << endl;
	return Load();
}

TableInfo Table::Show() const{
	return info;
}

void Table::Read(){
	FILE *f = fopen( FileName(), "rb" );

	// read table information.
	info.Read( f );
	// read the data of table.
	buf = new char[info.lineNum * info.colWidth];
	fread( buf, info.lineNum, info.colWidth, f );
	fclose( f );
}

void Table::Write(){
	FILE *f = fopen( FileName(), "wb");
	info.Write( f );
	if( buf ){
		fwrite( buf, info.lineNum, info.colWidth,  f);
	}
	fclose( f );
}

bool Table::NoRef( int colNum, string elem ){
	if( strcmp(name, "ref" )==0 ){
		return false;
	}
	if( strcmp(name, "BenSQL" )==0 ){
		return false;
	}
	string testRefQuery = "select to_table, to_col from ref where from_table =";
	testRefQuery += name;
	testRefQuery += " && from_col = ";
	testRefQuery += info.col[colNum].name;
	testRefQuery += ";\nquit;\n";
	std::istringstream testRefSS(testRefQuery);
	Core testRefCore(testRefSS,false);
	testRefCore.Run();

	vector<int> lines = testRefCore.printer.lastLines;
	vector<int> cols = testRefCore.printer.lastCols;
	Table ref("ref");
	ref.Read();
	for( unsigned int j=0; j< lines.size(); j++ ){
		char * refTable = ref.buf + lines[j]*ref.info.colWidth + ref.info.col[cols[0]].index;
		char * refCol = ref.buf + lines[j]*ref.info.colWidth + ref.info.col[cols[1]].index;
		string query = "select * from ";
		query += refTable;
		query += " where ";
		query += refCol;
		query += " = '";
		query += elem;
		query += "'";
		query += ";\nquit;\n";
		std::istringstream ss(query);
		Core test(ss, false);
//			cout << query1;
		test.Run();
		if( test.printer.lastLines.size() == 0 ){
			err << "Sorry. " << elem << " not exsit in " << refTable << "." << refCol << "\n";
			return true;
		}
	}
	return false;
}
bool Table::IsRef( int colNum, string elem ){
	if( strcmp(name, "ref" )==0 ){
		return false;
	}
	if( strcmp(name, "BenSQL" )==0 ){
		return false;
	}
	string testRefQuery = "select from_table, from_col from ref where to_table =";
	testRefQuery += name;
	testRefQuery += " && to_col = ";
	testRefQuery += info.col[colNum].name;
	testRefQuery += ";\nquit;\n";
	std::istringstream testRefSS(testRefQuery);
	Core testRefCore(testRefSS,false);
	testRefCore.Run();

	vector<int> lines = testRefCore.printer.lastLines;
	vector<int> cols = testRefCore.printer.lastCols;
	Table ref("ref");
	ref.Read();
	for( unsigned int j=0; j< lines.size(); j++ ){
		char * refTable = ref.buf + lines[j]*ref.info.colWidth + ref.info.col[cols[0]].index;
		char * refCol = ref.buf + lines[j]*ref.info.colWidth + ref.info.col[cols[1]].index;
		string query = "select * from ";
		query += refTable;
		query += " where ";
		query += refCol;
		query += " = '";
		query += elem;
		query += "'";
		query += ";\nquit;\n";
		std::istringstream ss(query);
		Core test(ss, false);
//			cout << query1;
		test.Run();
		if( test.printer.lastLines.size() > 0 ){
			err << "Sorry. " << elem << " is referd by " << refTable << "." << refCol << "\n";
			return true;
		}
	}
	return false;
}

bool Table::Insert( vector<string> elem ){
	Read();

	char *line = new char[info.colWidth];

	string testKeyQuery = "select * from ";
	testKeyQuery += name;
	testKeyQuery += " where 1 ";

	for( int i=0; i< info.colNum; i++ ){
		if( info.col[i].isKey ){
			testKeyQuery += " && ";
			testKeyQuery += info.col[i].name;
			testKeyQuery += " = '";
			testKeyQuery += elem[i];
			testKeyQuery += "'";
		}
		if( info.col[i].isNotNull && elem[i].empty() ){
			err << "Sorry. " << info.col[i].name << " can\'t be null.\n";
			return false;
		}
		if( info.col[i].isInteger ){
			for( unsigned int j=0; j<elem[i].size(); j++ ){
				if(  !isdigit(elem[i][j]) && (elem[i][j] != '-' || j!=0 ) ){
					cout << "Sorry. " << info.col[i].name << " must be integer." << endl;
					return false;
				}
			}
		}

		if( NoRef( i, elem[i] ) ){
			return false;
		}

		strncpy( (line+info.col[i].index ), elem[i].c_str(), info.col[i].length -1 );
	}
	if( testKeyQuery.find('=') +1 ){
//		cout << testKeyQuery;
		testKeyQuery += ";\nquit;\n";
		std::istringstream testKeySS(testKeyQuery);
		Core testKeyCore(testKeySS, false);
		testKeyCore.Run();
		if( testKeyCore.printer.lastLines.size() > 0 ){
			err << "Sorry. Key must be unique.\n";
			return false;
		}
	}

	if( ! info.checker.check(line) ){
		err << "Sorry. Miss " << name << " assume \n";
		return false;
	}


	FILE *f = fopen( FileName(), "wb");
	info.lineNum += 1;
	info.Write( f );
	if( buf ){
		fwrite( buf, info.lineNum-1, info.colWidth,  f);
	}
	fwrite( line, info.colWidth, 1, f );
	fclose( f );

	return true;

}

bool Table::Delete( vector<int> lines ){
	Read();

	for(unsigned int i=0; i<lines.size(); i++ ){
		for( int j=0; j<info.colNum; j++ ){
			if( IsRef(j, buf + lines[i]*info.colWidth + info.col[j].index ) ){
				return false;
			}
		}
	}

	FILE *f = fopen( FileName(), "wb");

	info.lineNum -= lines.size();
	info.Write( f );
	info.lineNum += lines.size();

	std::sort( lines.begin(), lines.end());	// 行号从小到大排列
	lines.insert( lines.begin(), -1 );		//	哨兵
	lines.push_back( info.lineNum );		//	哨兵
	for(unsigned int i=1; i<lines.size(); i++ ){
		fwrite( buf + (lines[i-1]+1)*info.colWidth, lines[i]-lines[i-1]-1, info.colWidth, f );
	}
	fclose( f );
	return true;
}


void Table::Select( vector<int> &selected, const vector<string> &condition ){
	Read();
	Check c( condition, info.col, info.colNum );
	for( int i=0; i< info.lineNum; i++ ){
		if( c.check( buf + i*info.colWidth ) ){
			selected.push_back( i );
		}
	}
}

void Table::Select( vector<int> &selected ){
	Read();
	for( int i=0; i< info.lineNum; i++ ){
		selected.push_back( i );
	}
}

bool Table::Update( vector<int> lines, vector<int> cols, vector<string> value  ){
	Read();
	for(unsigned int i=0; i<lines.size(); i++ ){
		for( unsigned int j=0; j<cols.size(); j++ ){
			if( NoRef(j, value[j]) ){
				return false;
			}
			char *oldValue = buf + lines[i]*info.colWidth + info.col[cols[j]].index;
			if( IsRef(j, oldValue ) ){
				return false;
			}
			strncpy( oldValue, value[j].c_str(), info.col[cols[j]].length-1 );
		}
		if( ! info.checker.check(buf + lines[i]*info.colWidth) ){
			err << "Sorry. Miss " << name << " assume \n";
			return false;
		}
	}
	Write();
	return true;
}

void Table::ColNum( vector<int> &cols ){
	Read();
	for( int i=0; i< info.colNum; i++ ){
		cols.push_back( i );
	}
}

void Table::ColNum( vector<int> &coln, const vector<string> &cols ){
	Read();
	coln.clear();
	for( unsigned int i=0; i< cols.size(); i++ ){
		int j;
		for( j=0; j< info.colNum; j++ ){
			if( cols[i].compare( info.col[j].name ) == 0 ){
				coln.push_back( j );
				break;
			}
		}
	}
}
