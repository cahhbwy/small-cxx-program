/*
 * ColInfo.h
 *
 *  Created on: 2010-6-9
 *      Author: Hui
 */

#ifndef COLINFO_H_
#define COLINFO_H_

#include "base.h"
class ColInfo{
public:
	char 	name[NAME_MAX_LEN];		// 列名
	int 	length;					// 最大长度
	int		index;					// 下标起始位置
	bool	isInteger;				// 整形
	bool	isNotNull;				// 非空
	bool	isKey;					// 关键字

	void Set( char *newName, int newLength, bool key =false, bool integer = false, bool notNull = false );
};

#endif /* COLINFO_H_ */
