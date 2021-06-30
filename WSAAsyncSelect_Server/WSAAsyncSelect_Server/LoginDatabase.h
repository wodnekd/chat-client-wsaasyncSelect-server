#pragma once
#include<mysql.h>
class CLoginDatabase
{
private:
	MYSQL*		 m_connection;					//MYSQL : Database와의 연결을 관리하는 구조체
	MYSQL		 m_conn;
	MYSQL_RES*	 m_sqlResult;					//MYSQL_RES   : SELECT 등 결과를 리턴하는 query의 결과를 나타내는 자료형
	MYSQL_ROW    m_sqlRow;						//MYSQL_ROW   : MYSQL_RES에서 하나의 레코드씩 값
	BOOL m_idCheck;
	char m_id[10];
	char m_password[20];
	char m_query[255];
	int m_queryState;
public:
	CLoginDatabase();
	~CLoginDatabase();
	BOOL ConnectDatabase();
	BOOL LoginUserID(LPWSTR loginID, LPWSTR pass);
	BOOL CreateUserID(LPWSTR loginID, LPWSTR Pass, LPWSTR nickname, LPWSTR questNum);
	LPWSTR SearchPW(LPWSTR loginID, LPWSTR questNum);
	LPWSTR GetNickname(LPWSTR loginID);
	BOOL CompareUserID(LPWSTR loginID);
	BOOL CompareUserNickname(LPWSTR nickname);
	BOOL DeleteUserID();
	
};

