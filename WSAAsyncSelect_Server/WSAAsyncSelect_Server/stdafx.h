// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#pragma comment(lib, "ws2_32")
#pragma warning(disable: 4996)
//에러 #define
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include<winsock2.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <locale.h>
#include<iostream>

using namespace std;


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "resource.h"
#include "Protocal.h"
#include "LoginDatabase.h"
#include "ClientManager.h"



//// 값에 문자열 추가해서 저장하기
//ChatStrData_RP chatStr;
//WCHAR findId[10] = L"";
//wcscpy(findId, findRoomNamePtr->FindId(wParam));
//swprintf(chatStr.chatData.str, 40, L"%s : %s", findId, strData_RQ.str);

//값 출력
//DisplayText(L"접속한 아이디 : %s\r\n", singleData.id);