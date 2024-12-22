#include "UserRecord.h"

using namespace std;
using namespace cocos2d;



char*  readBack ;

int loadRecordCount(void* para, int n_column, char** column_value, char** column_name)  {     
	int *pCount=(int*)para;     
	sscanf(column_value[0], "%d", pCount);     
	return 0;  
}
int loadUserRecord(void* para,int n_column,char** column_value,char **column_name)  
{      
	//UserRecord* record = (UserRecord*)para;     
	//record->setCoin(coin);     
	//record->setExp(experience); 
	//readBack = column_value[2];
	//CCLOG("key:%s :value:%s",column_value[1],column_value[2]);  
	//CCLOG("column_value[2]:%s",column_value[2]);  

	readBack = new char[strlen(column_value[2])+1];
    strcpy(readBack, column_value[2]);

	return 0;  
} 

UserRecord::UserRecord(const char *dbFilename,const char* table){
	openTableInDB(dbFilename,table);
}
//sqlite3_exec(pDB, sql, loadUserRecord, this, &errorMsg);//pDBһ���Ѵ򿪵����ݿ� ,sql��Ҫִ�е�SQL���,loadUserRecord�ص�����, this�ص������ĵ�һ�����������ڴ����Զ�������),errorMsg����ʱ���صĴ�����Ϣ  

/*
  �����ݿ�dbFilename��������ݿ��ļ������ڣ�����Զ��������򿪳ɹ������Ŀ����table�����ڣ��򴴽����
*/
void  UserRecord::openTableInDB(const char *dbFilename,const char* table) 
{      
	//sqlite3 *pDB = NULL;  

	int result = 1;

    char *errorMsg = NULL;  

	this->table = table;
	//string pauth= CCFileUtils::sharedFileUtils()->getWriteablePath()+(string)dbFilename;
	
	string pauth= CCFileUtils::sharedFileUtils()->getWritablePath()+(string)dbFilename;
	CCLOG("pauth:%s",pauth.c_str()); 
	if(SQLITE_OK != sqlite3_open(pauth.c_str(), &sqlDB)) {          
		CCLOG("open sql file failed!");          
		return ;      
	}  
	
	string sql = "create table if not exists " + string(table) + "(Id integer primary key,name,valus)";      
	sqlite3_exec(sqlDB, sql.c_str(), NULL, NULL, &errorMsg);     
	if(errorMsg != NULL) {         
		CCLOG("exec sql %s fail with msg: %s", sql.c_str(), errorMsg);          
		sqlite3_close(sqlDB);          
		return ;     
	}      
}

void UserRecord::saveToSQLite(const char* key,const char* valus)  {     
	char sql[512];      
	//sqlite3* pDB = createTableInDB(table, dbFilename);     
	if(sqlDB!=NULL) {          
		int count = 0;          
		char *errorMsg;           
		sprintf(sql, "select * from %s where name = '%s'", table, key);  
		//CCLOG("1sql:%s",sql);
		sqlite3_exec(sqlDB, sql, loadRecordCount, &count, &errorMsg);           
		if(errorMsg != NULL) {              
			CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);              
			sqlite3_close(sqlDB);              
			return;          
		}          
		if(count){
			sprintf(sql, "update %s set valus = '%s' where name = '%s'", table,valus,key);         
		} else {              
			sprintf(sql, "insert into %s(name,valus) values('%s','%s')",table,key,valus);         
		}  

		//CCLOG("2sql:%s",sql);

		sqlite3_exec(sqlDB, sql, NULL, NULL, &errorMsg);         
		if(errorMsg != NULL){             
			CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);              
			sqlite3_close(sqlDB);             
			return;          
		}      
	}       
	
}   


void UserRecord::saveInt(const char* key,int valus){
	char va[128];
	sprintf (va,"%d", valus); 
	saveToSQLite(key,va);

}

void UserRecord::saveFloat(const char* key,float valus){
	char va[128];
	sprintf (va,"%f", valus); 
	saveToSQLite(key,va);

}
void UserRecord::saveString(const char* key,const char* valus){

	saveToSQLite(key,valus);

}
void UserRecord::saveBool(const char* key,bool valus){
	char va[128];
	sprintf (va,"%s", valus?"true":"false"); 
	saveToSQLite(key,va);

}

 /*
 �ر����ݿ�
 */
void UserRecord::closeDB(){
	sqlite3_close(sqlDB); 
}

void UserRecord::readFromSQLite(const char* key)  {      
	char sql[512];      
//	sqlite3* pDB = createTableInDB(table, dbFilename); 
	
	if(sqlDB != NULL) {          
		int count = 0;          
		char *errorMsg;          
		sprintf(sql,"select * from %s where name = '%s'", table,  key);          
		sqlite3_exec(sqlDB, sql, loadUserRecord, this, &errorMsg);          
		if(errorMsg!=NULL) {              
			CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);              
			sqlite3_close(sqlDB);              
			return;          
		}   
	} 
}
int UserRecord::readIntFromSQLite(const char* key){
	int  read;
	readFromSQLite(key);

	read = atoi(readBack);
	return read;
}


float UserRecord::readFloatFromSQLite(const char* key){
	float  read;
	readFromSQLite(key);

	//CCLOG("readFloatFromSQLite:%s",readBack);   
	read = atof(readBack);
	return read;
}

 char* UserRecord::readStringFromSQLite(const char* key){
	readFromSQLite(key);
	//CCLOG("readStringFromSQLite:%s",readBack);
	return readBack;
}


bool UserRecord::readBoolFromSQLite(const char* key){
	bool  read;
	readFromSQLite(key);

	//CCLOG("readBoolFromSQLite:%s",readBack);   
	if(string(readBack)=="true"){
		read = true;
	}else{
		read = false;
	}
	return read;
}