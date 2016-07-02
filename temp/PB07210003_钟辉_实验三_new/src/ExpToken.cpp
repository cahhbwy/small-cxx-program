/*
 * ExpToken.cpp
 *
 *  Created on: 2010-6-6
 *      Author: Hui
 */
#include "ExpToken.h"

// "" must be in the end;
// the order take effect on lexer.
const char *g_Optr[OPTR_MAX_NUM]= { "(",          ",", "#", ")", "||", "&&", "=", "<", ">", "+",  "-",  "*", "/", "%",  "" };
const int g_Prior[OPTR_MAX_NUM]=  { OPTR_MAX_NUM,  0,   1,   2,   3,    4,    5,   6,   6,   10,   10,   15,  15,  15,   0 };

ExpToken ExpToken::endOptr(){
		ExpToken e("#");
		return e;
}

double ExpToken::getNum(){
	istringstream instr(name);
	double num;
	instr >> num;
	return num;
}

ExpToken::ExpToken( string  e){
	name = e;
	for( optrNum=0; g_Optr[optrNum][0]; optrNum++ ){
		if( name.compare( g_Optr[optrNum] ) == 0 ){
			break;
		}
	}
}

ExpToken::~ExpToken(){
}
void ExpToken::eval(stack<ExpToken> &opnd ){
	if( name.compare( "||" ) == 0 ){
		string right = opnd.top().getName();
		opnd.pop();
		string left = opnd.top().getName();
		opnd.pop();
		std::ostringstream ss;
		ss << (left.compare("0") || right.compare("0"));
		opnd.push( ss.str() );
	}
	else if( name.compare( "&&" ) == 0 ){
		string right = opnd.top().getName();
		opnd.pop();
		string left = opnd.top().getName();
		opnd.pop();
		std::ostringstream ss;
		ss << (left.compare("0") && right.compare("0"));
		opnd.push( ss.str() );
	}
	else if( name.compare( "=" ) == 0 ){
		string right = opnd.top().getName();
		opnd.pop();
		string left = opnd.top().getName();
		opnd.pop();
		std::ostringstream ss;
		ss << (left.compare(right) == 0);
		opnd.push( ss.str() );
	}
	else if( name.compare( ">" ) == 0 ){
		double right = opnd.top().getNum();
		opnd.pop();
		double left = opnd.top().getNum();
		opnd.pop();
		std::ostringstream ss;
		ss << (left > right);
		opnd.push( ss.str() );
	}
	else if( name.compare( "<" ) == 0 ){
		double right = opnd.top().getNum();
		opnd.pop();
		double left = opnd.top().getNum();
		opnd.pop();
		std::ostringstream ss;
		ss << (left < right);
		opnd.push( ss.str() );
	}
	else if( name.compare( "-" ) == 0 ){
		double right = opnd.top().getNum();
		opnd.pop();
		double left = opnd.top().getNum();
		opnd.pop();
		std::ostringstream ss;
		ss << left - right;
		opnd.push( ss.str() );
	}
	else if( name.compare( "*" ) == 0 ){
		double right = opnd.top().getNum();
		opnd.pop();
		double left = opnd.top().getNum();
		opnd.pop();
		std::ostringstream ss;
		ss << left * right;
		opnd.push( ss.str() );
	}
	else if( name.compare( "/" ) == 0 ){
		double right = opnd.top().getNum();
		opnd.pop();
		double left = opnd.top().getNum();
		opnd.pop();
		std::ostringstream ss;
		ss << left / right;
		opnd.push( ss.str() );
	}
	else if( name.compare( "%" ) == 0 ){
		double right = opnd.top().getNum();
		opnd.pop();
		double left = opnd.top().getNum();
		opnd.pop();
		std::ostringstream ss;
		ss << (int)left % (int)right;
		opnd.push( ss.str() );
	}
}
bool ExpToken::isOpnd(){
	return getPrior()==0;
}
bool ExpToken::isOptr(){
	return getPrior()>0;
}
int ExpToken::getPrior(){
	return g_Prior[optrNum];
}
string ExpToken::getName(){
	return name;
}
