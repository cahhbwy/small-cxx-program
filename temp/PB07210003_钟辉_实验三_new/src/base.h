/*
 * base.h
 *
 *  Created on: 2010-6-4
 *      Author: Hui
 */

#ifndef BASE_H_
#define BASE_H_

#include "type.h"
#include "config.h"
#include <cstdlib>
#include <iostream>
#include <cstring>
using namespace std;

extern char gTmpStr[ TMP_STR_SIZE ];

void Error(char *msg);

#endif /* BASE_H_ */
