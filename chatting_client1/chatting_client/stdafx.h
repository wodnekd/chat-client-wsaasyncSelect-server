// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#pragma warning(disable: 4996)
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include<winsock2.h>
// C 런타임 헤더 파일입니다.
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.



//사용 문법 

//받은 메세지 출력
//SendDlgItemMessage(hDlg, IDC_CHAT_RECV_EDIT, EM_REPLACESEL, FALSE, (LPARAM)L"ok버튼");
//접속자 목록 추가
//SendDlgItemMessage(hDlg, IDC_CHAT_LIST, LB_ADDSTRING, 0, (LPARAM)L"ok버튼");
//내 아이디 등록
//SendDlgItemMessage(hDlg, IDC_CHAT_MY_ID_EDIT, EM_REPLACESEL, FALSE, (LPARAM)myId);

//리스트 클릭한 곳 좌표와 그 곳의 값 가져오기
//int idLocal = 0;
//WCHAR recvId[10] = { 0, };
//idLocal = SendMessage(hIdListOtheridListbox, LB_GETCURSEL, 0, 0);
//SendMessage(hIdListOtheridListbox, LB_GETTEXT, idLocal, (LPARAM)recvId);

//모델리스 다이얼로그 만들고 창 띄우기
//hDlg_SendMessageProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SEND_MESSAGE), hWnd, Dlg_SendMessageProc);
//ShowWindow(hDlg_SendMessageProc, SW_SHOW);

//메시지 박스에 변수 출력하는 방법
/*WCHAR text[20];
wsprintf(text, L"소켓 크기 : %s", roomName);
MessageBox(GetFocus(), text, L"회원 가입 결과", MB_OK);*/

//Set 과 Send 의 차이
//Set은 문자 입력할 떄
//Send는 리스트 박스 같은 것에 사용할 떄 // 메세지를 보냄
//SetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_MY_ID_EDIT, myId);
//SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, EM_REPLACESEL, FALSE, (LPARAM)myChatRoomName);