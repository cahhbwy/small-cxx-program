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
	char 	name[NAME_MAX_LEN];		// ����
	int 	length;					// ��󳤶�
	int		index;					// �±���ʼλ��
	bool	isInteger;				// ����
	bool	isNotNull;				// �ǿ�
	bool	isKey;					// �ؼ���

	void Set( char *newName, int newLength, bool key =false, bool integer = false, bool notNull = false );
};

#endif /* COLINFO_H_ */
