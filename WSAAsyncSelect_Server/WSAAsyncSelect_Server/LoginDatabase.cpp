#include "stdafx.h"
#include "LoginDatabase.h"

#pragma comment(lib, "libmysql.lib")
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "dblogin"
#define DB_NAME "test"
#define CHOP(x) x[strlen(x) - 1] = ' '

//
//	������ : CLoginDatabase()
//  
//	�� ��  : �α��� ������ ����ϱ� �� ������ �ʱ�ȭ
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
//	�ɹ� �Լ� : ConnectDatabase()
//
//	�� �� : ������ mysql�� ����
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
//	�ɹ� �Լ� : LoginUserID()
//
//	�� �� : �α��� �õ��� ������ ���̵�, ��й�ȣ�� DB�� ���Ͽ�
//		    �α��� ����, ���и� Ȯ��

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
	//�α��� ���̺� �÷��� �Ѳ����� ��������
	m_sqlResult = mysql_store_result(m_connection);

	while ((m_sqlRow = mysql_fetch_row(m_sqlResult)) != NULL)
	{
		if (strcmp(m_sqlRow[0], tempID) == 0)
		{
			m_idCheck = TRUE;
			if (strcmp(m_sqlRow[1], tempPW) == 0)
			{
				printf("�α��� ����\n");
				return TRUE;
			}
			else
			{
				printf("��й�ȣ ����\n");
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
		printf("���̵� �������� �ʽ��ϴ�.\n");
		return FALSE;
	}
	mysql_free_result(m_sqlResult);
	return TRUE;
}

//
//	�ɹ� �Լ� : CreateUserID()
//
//	�� �� : ������ ���̵�, ��й�ȣ�� �����Ͽ� ������ ����
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
//	�ɹ� �Լ� : SearchPW()
//
//	�� �� : ���� ���̵�� �ʱ� ���� �������� ��й�ȣ�� ã��
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
//	�ɹ� �Լ� : CompareUserID()
//
//	�� �� : ���� ���� �� ���̵� �ߺ��� ���� ���� ���̵� Ȯ��
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
//	�ɹ� �Լ� : CompareUserID()
//
//	�� �� : ���� ���� �� ���̵� �ߺ��� ���� ���� ���̵� Ȯ��
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
//	�ɹ� �Լ� : DeleteUserID()
//
//	�� �� : ������ ����
//
BOOL CLoginDatabase::DeleteUserID()
{
	printf("������ ���� ���̵� �Է� : ");
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
