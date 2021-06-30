#pragma once
#include<mysql.h>
class CDatabase
{
private:
	MYSQL*		 m_Connection;					//MYSQL : Database���� ������ �����ϴ� ����ü
	MYSQL		 m_Conn;
	MYSQL_RES*	 m_SqlResult;					//MYSQL_RES   : SELECT �� ����� �����ϴ� query�� ����� ��Ÿ���� �ڷ���
	MYSQL_ROW    m_SqlRow;						//MYSQL_ROW   : MYSQL_RES���� �ϳ��� ���ڵ徿 ���� ��� �ö� ���̴� �ڷ���

public:
	CDatabase();
	~CDatabase();
	BOOL			ConnectSql();
	MYSQL*			GetConnection() { return m_Connection; };
	MYSQL			GetConn() { return m_Conn; };
	MYSQL_RES*		GetSqlResult() { return m_SqlResult; };
	MYSQL_RES*		SetSqlResult(MYSQL_RES * mysql_store_result);
	MYSQL_ROW		SetSqlRow(MYSQL_ROW mysql_fetch_row);
	MYSQL_ROW		GetSqlRow() { return m_SqlRow; };

};
//mysql_query ������ �����ͺ��̽� ������ �ϳ��� ���Ǹ� ����
//MYSQL_RES* mysql_store_result(MYSQL* mysql) db���� data �Ѳ����� ��������
//MYSQL_ROW mysql_fetch_row(MYSQL_ROW* result) ������ data �� ����
//int mysql_num_fields(MYSQL_RES* result) �ʵ��� ���� �ľ�

