#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__
#include "cocos2d.h"

class StringUtil{

public:
	static const char* getString(const char *key){
		cocos2d::CCDictionary *strings = cocos2d::CCDictionary::createWithContentsOfFile("strings.xml");
		return ((cocos2d::CCString*)strings->objectForKey(key))->m_sString.c_str();
	}
};

#endif