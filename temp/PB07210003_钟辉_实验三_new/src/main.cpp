/*
 * main.cpp
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */
#include <string>
using namespace std;

#include "base.h"
#include "core.h"
#include "table.h"
#include "print.h"
#include "explain.h"
#include "ExpToken.h"


#include "calculator.h"
int main( int argc, char *argv[]){

//	argc = 2;

	if( argc > 1 ){
		ifstream fin("in.txt");
		Core cc( fin );
		cc.Run();
	}
	else{
		Core cc;
		cc.Run();
	}

	return 0;
}

