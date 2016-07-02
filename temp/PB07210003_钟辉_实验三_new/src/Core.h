/*
 * Core.h
 *
 *  Created on: 2010-6-5
 *      Author: Hui
 */

#ifndef CORE_H_
#define CORE_H_

#include "print.h"
#include "explain.h"
#include "table.h"
#include <vector>

using namespace std;


class Core {
	istream &in;
	Explain e;
public:
	Print printer;
	Core( istream &coreIn = cin, bool showMsg = true ):in( coreIn ), e(showMsg), printer(showMsg){};
	virtual ~Core();
	void Run();
};

#endif /* CORE_H_ */
