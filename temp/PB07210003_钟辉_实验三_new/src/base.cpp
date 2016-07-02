/*
 * base.cpp
 *
 *  Created on: 2010-6-5
 *      Author: Hui
 */

#include "base.h"

extern char gTmpStr[ TMP_STR_SIZE ];

void Error(char *msg){
	cout << msg;
	exit(-1);
}
