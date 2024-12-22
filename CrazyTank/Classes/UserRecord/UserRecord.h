#ifndef __USER_RECORD_H__
#define __USER_RECORD_H__

#include "cocos2d.h"
#include "../sqlite3/sqlite3.h"



class  UserRecord{

private:
	sqlite3* sqlDB;//数据库
	const char* table;//表明

	void  openTableInDB(const char *dbFilename,const char* table); 
	void  saveToSQLite(const char* key,const char* valus);  
	void  readFromSQLite(const char* key); 
public:

	UserRecord(const char *dbFilename,const char* table);

	void saveInt(const char* key,int valus);
	void saveFloat(const char* key,float valus);
	void saveString(const char* key,const char* valus);
	void saveBool(const char* key,bool valus);

	int    readIntFromSQLite(const char* key);
	float  readFloatFromSQLite(const char* key);
    char*  readStringFromSQLite(const char* key);
	bool   readBoolFromSQLite(const char* key);

	void closeDB();

};

#endif