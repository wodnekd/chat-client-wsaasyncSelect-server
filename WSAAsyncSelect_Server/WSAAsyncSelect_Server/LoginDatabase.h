#pragma once
#include<mysql.h>
class CLoginDatabase
{
private:
	MYSQL*		 m_connection;					//MYSQL : Database���� ������ �����ϴ� ����ü
	MYSQL		 m_conn;
	MYSQL_RES*	 m_sqlResult;					//MYSQL_RES   : SELECT �� ����� �����ϴ� query�� ����� ��Ÿ���� �ڷ���
	MYSQL_ROW    m_sqlRow;						//MYSQL_ROW   : MYSQL_RES���� �ϳ��� ���ڵ徿 ��
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

