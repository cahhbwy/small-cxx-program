/*
 * ColInfo.cpp
 *
 *  Created on: 2010-6-9
 *      Author: Hui
 */
#include "colinfo.h"

void ColInfo::Set( char *newName, int newLength, bool key, bool integer, bool notNull ){
	strncpy( name, newName, NAME_MAX_LEN-1 );
	length = newLength;
	isKey = key;
	isNotNull = notNull;
	isInteger = integer;
}


