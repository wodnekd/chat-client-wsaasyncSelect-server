// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#pragma comment(lib, "ws2_32")
#pragma warning(disable: 4996)
//���� #define
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>
#include<winsock2.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <locale.h>
#include<iostream>

using namespace std;


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "resource.h"
#include "Protocal.h"
#include "LoginDatabase.h"
#include "ClientManager.h"



//// ���� ���ڿ� �߰��ؼ� �����ϱ�
//ChatStrData_RP chatStr;
//WCHAR findId[10] = L"";
//wcscpy(findId, findRoomNamePtr->FindId(wParam));
//swprintf(chatStr.chatData.str, 40, L"%s : %s", findId, strData_RQ.str);

//�� ���
//DisplayText(L"������ ���̵� : %s\r\n", singleData.id);