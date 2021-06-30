#include "stdafx.h"
#include "LoginDatabase.h"

#pragma comment(lib, "libmysql.lib")
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "dblogin"
#define DB_NAME "test"
#define CHOP(x) x[strlen(x) - 1] = ' '

//
//	생성자 : CLoginDatabase()
//  
//	목 적  : 로그인 정보를 사용하기 전 변수들 초기화
//
CLoginDatabase::CLoginDatabase()
{
	m_idCheck = false;
	memset(m_id, 0, sizeof(m_id));
	memset(m_password, 0, sizeof(m_password));
	memset(m_query, 0, sizeof(m_query));
	m_queryState = 0;
	ConnectDatabase();
}


CLoginDatabase::~CLoginDatabase()
{

}

//
//	맴버 함수 : ConnectDatabase()
//
//	목 적 : 서버와 mysql을 연결
//
BOOL CLoginDatabase::ConnectDatabase()
{
	mysql_init(&m_conn);

	m_connection = mysql_real_connect(&m_conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	if (m_connection == NULL)
	{
		fprintf(stderr, "Mysql connection err : %s", mysql_error(&m_conn));
		return FALSE;
	}
	mysql_query(m_connection, "set names euckr");
	return TRUE;
}

//
//	맴버 함수 : LoginUserID()
//
//	목 적 : 로그인 시도한 유저의 아이디, 비밀번호를 DB와 비교하여
//		    로그인 성공, 실패를 확인

BOOL CLoginDatabase::LoginUserID(LPWSTR loginID, LPWSTR pass)
{
	char tempID[10] = "";
	char tempPW[20] = "";
	int len;
	

	len = WideCharToMultiByte(CP_ACP, 0, loginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, loginID, -1, tempID, len, NULL, NULL);

	len = WideCharToMultiByte(CP_ACP, 0, pass, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, pass, -1, tempPW, len, NULL, NULL);
	
	m_queryState = mysql_query(m_connection, "select * from login");
	if (m_queryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&m_conn));
		return FALSE;
	}
	//로그인 테이블 컬럼을 한꺼번에 가져오기
	m_sqlResult = mysql_store_result(m_connection);

	while ((m_sqlRow = mysql_fetch_row(m_sqlResult)) != NULL)
	{
		if (strcmp(m_sqlRow[0], tempID) == 0)
		{
			m_idCheck = TRUE;
			if (strcmp(m_sqlRow[1], tempPW) == 0)
			{
				printf("로그인 성공\n");
				return TRUE;
			}
			else
			{
				printf("비밀번호 실패\n");
				return FALSE;
			}
		}
		else
		{
			m_idCheck = FALSE;
		}
	}
	if (m_idCheck == FALSE)
	{
		printf("아이디가 존재하지 않습니다.\n");
		return FALSE;
	}
	mysql_free_result(m_sqlResult);
	return TRUE;
}

//
//	맴버 함수 : CreateUserID()
//
//	목 적 : 유저가 아이디, 비밀번호를 성정하여 계정을 생성
//

BOOL CLoginDatabase::CreateUserID(LPWSTR loginID, LPWSTR pass, LPWSTR nickname, LPWSTR questNum)
{
	char tempID[10] = "";
	char tempPW[10] = "";
	char tempNickname[10] = "";
	char tempquestNum[10] = "";
	
	int len = 0;
	
	len = WideCharToMultiByte(CP_ACP, 0, loginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, loginID, -1, tempID, len, NULL, NULL);

	len = WideCharToMultiByte(CP_ACP, 0, pass, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, pass, -1, tempPW, len, NULL, NULL);

	len = WideCharToMultiByte(CP_ACP, 0, nickname, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, nickname, -1, tempNickname, len, NULL, NULL);

	len = WideCharToMultiByte(CP_ACP, 0, questNum, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, questNum, -1, tempquestNum, len, NULL, NULL);
	
	sprintf(m_query, "insert into login values" "('%s','%s','%s','%s')", tempID, tempPW, tempNickname, tempquestNum);

	m_queryState = mysql_query(m_connection, m_query);
	if (m_queryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s \n", mysql_error(&m_conn));
		return FALSE;
	}
	return TRUE;
}

//
//	맴버 함수 : SearchPW()
//
//	목 적 : 유저 아이디와 초기 설정 질문으로 비밀번호를 찾기
//

LPWSTR CLoginDatabase::SearchPW(LPWSTR loginID, LPWSTR questNum)
{
	char tempID[10] = "";
	char tempPW[10] = "";
	char tempquestNum[10] = "";
	WCHAR wtempPW[10] = L"";
	int len;
	
	len = WideCharToMultiByte(CP_ACP, 0, loginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, loginID, -1, tempID, len, NULL, NULL);

	len = WideCharToMultiByte(CP_ACP, 0, questNum, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, questNum, -1, tempquestNum, len, NULL, NULL);
	
	m_queryState = mysql_query(m_connection, "select * from login");
	m_sqlResult = mysql_store_result(m_connection);

	while ((m_sqlRow = mysql_fetch_row(m_sqlResult)) != NULL)
	{
		if (strcmp(m_sqlRow[0], tempID) == 0)
		{
			if (strcmp(m_sqlRow[3], tempquestNum) == 0)
			{
				strcpy(tempPW, m_sqlRow[1]);
				len = MultiByteToWideChar(CP_ACP, 0, tempPW, strlen(tempPW), NULL, NULL);
				MultiByteToWideChar(CP_ACP, 0, tempPW, strlen(tempPW), wtempPW, len);
				return wtempPW;
			}
			else
			{
				return wtempPW;
			}
		}
	}
	return wtempPW;
}

LPWSTR CLoginDatabase::GetNickname(LPWSTR loginID)
{
	char tempID[10] = "";
	char tempNickname[10] = "";
	WCHAR wtempNickname[10] = L"";
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, loginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, loginID, -1, tempID, len, NULL, NULL);

	m_queryState = mysql_query(m_connection, "select * from login");
	m_sqlResult = mysql_store_result(m_connection);

	while ((m_sqlRow = mysql_fetch_row(m_sqlResult)) != NULL)
	{
		if (strcmp(m_sqlRow[0], tempID) == 0)
		{
			strcpy(tempNickname, m_sqlRow[2]);
			len = MultiByteToWideChar(CP_ACP, 0, tempNickname, strlen(tempNickname), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, tempNickname, strlen(tempNickname), wtempNickname, len);
			return wtempNickname;
		}
	}
	return wtempNickname;
}

//
//	맴버 함수 : CompareUserID()
//
//	목 적 : 계정 생성 시 아이디 중복을 막기 위해 아이디 확인
//

BOOL CLoginDatabase::CompareUserID(LPWSTR loginID)
{
	char tempID[10] = "";
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, loginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, loginID, -1, tempID, len, NULL, NULL);

	m_queryState = mysql_query(m_connection, "select * from login");
	m_sqlResult = mysql_store_result(m_connection);
	
	while ((m_sqlRow = mysql_fetch_row(m_sqlResult)) != NULL)
	{
		if (strcmp(m_sqlRow[0], tempID) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//
//	맴버 함수 : CompareUserID()
//
//	목 적 : 계정 생성 시 아이디 중복을 막기 위해 아이디 확인
//

BOOL CLoginDatabase::CompareUserNickname(LPWSTR nickname)
{
	char tempNickname[10] = "";
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, nickname, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, nickname, -1, tempNickname, len, NULL, NULL);

	m_queryState = mysql_query(m_connection, "select * from login");
	m_sqlResult = mysql_store_result(m_connection);

	while ((m_sqlRow = mysql_fetch_row(m_sqlResult)) != NULL)
	{
		if (strcmp(m_sqlRow[1], tempNickname) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//
//	맴버 함수 : DeleteUserID()
//
//	목 적 : 계정을 삭제
//
BOOL CLoginDatabase::DeleteUserID()
{
	printf("삭제할 유저 아이디 입력 : ");
	fgets(m_id, 12, stdin);
	CHOP(m_id);

	sprintf(m_query, "delete from login where id = '%s' ", m_id);

	m_queryState = mysql_query(m_connection, m_query);

	if (m_queryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&m_conn));
		return FALSE;
	}
	return TRUE;
}
