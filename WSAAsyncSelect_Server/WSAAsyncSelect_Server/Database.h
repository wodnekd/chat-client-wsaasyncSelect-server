#pragma once
#include<mysql.h>
class CDatabase
{
private:
	MYSQL*		 m_Connection;					//MYSQL : Database와의 연결을 관리하는 구조체
	MYSQL		 m_Conn;
	MYSQL_RES*	 m_SqlResult;					//MYSQL_RES   : SELECT 등 결과를 리턴하는 query의 결과를 나타내는 자료형
	MYSQL_ROW    m_SqlRow;						//MYSQL_ROW   : MYSQL_RES에서 하나의 레코드씩 값을 얻어 올때 쓰이는 자료형

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
//mysql_query 지정한 데이터베이스 서버에 하나의 질의를 전송
//MYSQL_RES* mysql_store_result(MYSQL* mysql) db에서 data 한꺼번에 가져오기
//MYSQL_ROW mysql_fetch_row(MYSQL_ROW* result) 가져온 data 에 접근
//int mysql_num_fields(MYSQL_RES* result) 필드의 갯수 파악


