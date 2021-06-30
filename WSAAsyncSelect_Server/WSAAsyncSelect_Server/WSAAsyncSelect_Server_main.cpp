// WSAAsyncSelect_Server.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


#define MAX_LOADSTRING 100
//server data 
#define SERVERPORT 9000
#define BUFSIZE	   512
#define WM_SOCKET (WM_USER+1)

// 전역 변수
HINSTANCE	hInst;                                // 현재 인스턴스입니다.
WCHAR		szTitle[MAX_LOADSTRING];              // 제목 표시줄 텍스트입니다.
WCHAR		szWindowClass[MAX_LOADSTRING];        // 기본 창 클래스 이름입니다.
HWND		hEdit;
// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//서버 처리 함수
void				InitServer(HWND hWnd);
void				ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
//error 출력 함수
void DisplayText(LPWSTR fmt, ...);
void err_quit(LPWSTR msg);
void err_display(LPWSTR msg);
void err_display(int errcode);
//클라이언트 관리 클레스
ClientManager CM_clientList;
//ClientManager CM_chatRoomClientList;
ChattingRoomManager CRM_chattingRoomList;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WSAASYNCSELECT_SERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WSAASYNCSELECT_SERVER));

	MSG msg;
	_wsetlocale(LC_ALL, L"korean");
	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WSAASYNCSELECT_SERVER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WSAASYNCSELECT_SERVER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		1000, 200, 500, 500, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		hEdit = CreateWindowW(L"edit", NULL,
			WS_CHILD | WS_VISIBLE |
			WS_HSCROLL | WS_VSCROLL |
			ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
			0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);
		//윈속 초기화 및 소켓 생성 및 listen() 과정까지
		InitServer(hWnd);
		return 0;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_SOCKET:
		//소켓 관련 메세지 처리
		ProcessSocketMessage(hWnd, message, wParam, lParam);
		return 0;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//
//  함수: InitServer(HWND hWnd)
//
//  목적:  서버 소켓 생성에 관련된 과정
//
//  설명:  윈속 생성 및 초기화, 
//        소켓 생성 및 bind,listen 과정, 
//		  WSAAsyncSelect() 사용하여 listen_sock 등록
void InitServer(HWND hWnd)
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	return;
	//socket 생성 AF_INET, SOCK_STREAM
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit(L"sock()");
	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	if (bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) err_quit(L"bind()");
	//listen()
	if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) err_quit(L"listen()");
	//WSAAsyncSelect()
	if (WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR) err_quit(L"WSAAsyncSelect()");

	DisplayText(L"Server Success!\r\n");
}


void ProcessSocketMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SOCKET		clientSock;
	SOCKADDR_IN clientAddr;
	int			addrLen;
	int			retval;
	Packet		header;
	if (WSAGETSELECTERROR(lParam))
	{
		DisplayText(L"오류 발생 \r\n");
		err_display(WSAGETSELECTERROR(lParam));
		CM_clientList.RemoveClientInfo(wParam, SEND_OTHER_ID_OUT_NF);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		addrLen = sizeof(clientAddr);
		clientSock = accept(wParam, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			err_display(L"accpt()");
			return;
		}
		DisplayText(L"[TCP 서버] : Client 접속 \r\n IP 주소 : %S, 포트 번호 : %d\r\n",
			inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		if (WSAAsyncSelect(clientSock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
		{
			err_display(L"WSAAsyncSelect(), clientSock");
		}
		break;
	}
	case FD_READ:
		//데이터 받기
		DisplayText(L"Header 받기 \r\n");
		retval = recv(wParam, (char*)&header, sizeof(Packet), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display(L"Packet recv()");
			return;
		}
		DisplayText(L"[HEAD] RECV SUCCESS  \r\n");

		switch (header.header)
		{
		case SIGNUP_RQ:
		{
			//회원 가입 데이터 받기
			DisplayText(L"SIGNUP_RQ 받기 \r\n");
			SignUpData signupData;
			retval = recv(wParam, (char*)&signupData, sizeof(signupData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"signupData recv()");
				err_display(WSAGetLastError());
				return;
			}
			DisplayText(L"[RECV] -> [SIGNUP_RQ] SUCCESS  \r\n");
			//회원 가입 성공 유무 확인
			SignUp_RP signup_RP;
			CLoginDatabase CdatabasesData;
			signup_RP.signupSucess = (CdatabasesData.CreateUserID(signupData.createID, signupData.createPW, signupData.createNickname, signupData.createQuest) != 0);
			//결과 전송
			retval = send(wParam, (char*)&signup_RP, sizeof(signup_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"signup_RP send()");
				return;
			}
			DisplayText(L"SIGNUP_RQ 에서 [SignUp_RP] -> [SEND]\r\n");
		}
		break;
		case LOGIN_RQ:
		{
			//로그인 데이터 받기
			DisplayText(L"LOGIN_RQ 받기 \r\n");
			LoginData loginData;
			retval = recv(wParam, (char*)&loginData, sizeof(loginData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"loginData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [LOGIN_RQ] SUCCESS  \r\n");
			//로그인 성공 여부 패킷 전송
			Login_RP login_RP;
			CLoginDatabase CLogin;
			wcscpy(login_RP.loginData_RP.nickname, CLogin.GetNickname(loginData.loginID));
			login_RP.loginData_RP.success = (CLogin.LoginUserID(loginData.loginID, loginData.loginPW) != 0);
			retval = send(wParam, (char*)&login_RP, sizeof(login_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"login_RP send()");
				return;
			}
			DisplayText(L"LOGIN_RQ 에서 [Login_RP] -> [SEND]\r\n");
			//로그인 성공 시 로그인 한 클라 닉네임 다른 클라에게 알려주기
			CLoginDatabase CdatabasesData;
			WCHAR nickname[10] = L"";
			wcscpy(nickname, CdatabasesData.GetNickname(loginData.loginID));
			if (login_RP.loginData_RP.success == TRUE)
			{
				CM_clientList.AddClientInfo(wParam, nickname);
				OtherId_In_NF sendOtherId_In_NF;
				ClientInfoNode *ptr = CM_clientList.head;
				while (ptr)
				{
					DisplayText(L"클라에게 id 배포 중 \r\n");
					if (ptr->sock != wParam)
					{
						wcscpy(sendOtherId_In_NF.otherIdInData.otherID, nickname);
						DisplayText(L"다른 클라에게 현재 접속한 id 보내기 %s \r\n", sendOtherId_In_NF.otherIdInData.otherID);
						send(ptr->sock, (char*)&sendOtherId_In_NF, sizeof(sendOtherId_In_NF), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"sendOtherId_NF send()");
							return;
						}
						DisplayText(L"LOGIN_RQ 에서 [OtherId_In_NF] -> [SEND]\r\n");
						memset(sendOtherId_In_NF.otherIdInData.otherID, 0, sizeof(sendOtherId_In_NF.otherIdInData.otherID));
						wcscpy(sendOtherId_In_NF.otherIdInData.otherID, ptr->id);
						DisplayText(L"현재 클라에게 기존에 접속했던 id 보내기 %s \r\n", sendOtherId_In_NF.otherIdInData.otherID);
						send(wParam, (char*)&sendOtherId_In_NF, sizeof(sendOtherId_In_NF), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"sendOtherId_NF send()");
							return;
						}
						DisplayText(L"LOGIN_RQ 에서 [OtherId_In_NF] -> [SEND]\r\n");
						ptr = ptr->next;
					}
					else if (ptr->sock == wParam)
					{
						ptr = ptr->next;
						continue;
					}

				}
			}

		}
		break;
		case SEARCH_PW_RQ:
		{
			SearchPWData_RQ searchPwData;
			SearchPW_RP searchPw_RP;
			CLoginDatabase CdatabasesData;
			retval = recv(wParam, (char*)&searchPwData, sizeof(searchPwData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"searchPwData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [SEARCH_PW_RQ] SUCCESS  \r\n");

			wcscpy(searchPw_RP.searchPwData.searchPW, CdatabasesData.SearchPW(searchPwData.loginID, searchPwData.loginQuest));
			if (lstrlen(searchPw_RP.searchPwData.searchPW) != 0)
			{
				searchPw_RP.searchPwData.searchSuccess = TRUE;
			}
			else if (lstrlen(searchPw_RP.searchPwData.searchPW) == 0)
			{
				searchPw_RP.searchPwData.searchSuccess = FALSE;
			}
			retval = send(wParam, (char*)&searchPw_RP, sizeof(searchPw_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"searchPw_RP send()");
				return;
			}
			DisplayText(L"SEARCH_PW_RQ 에서 SearchPW_RP -> [SEND]\r\n");
		}
		break;
		case COMPARE_ID_RQ:
		{
			CompareData compareData;
			CompareID_RP compareId_RP;
			CLoginDatabase CdatabasesData;
			retval = recv(wParam, (char*)&compareData, sizeof(compareData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"compareData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [COMPARE_ID_RQ] SUCCESS  \r\n");

			compareId_RP.compareIdSueccess = (CdatabasesData.CompareUserID(compareData.compareChar) != 0);
			retval = send(wParam, (char*)&compareId_RP, sizeof(compareId_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"compareId_RP send()");
				return;
			}
			DisplayText(L"COMPARE_ID_RQ 에서 [CompareID_RP] -> [SEND]\r\n");
		}
		break;
		case COMPARE_NICKNAME_RQ:
		{
			CompareData compareData;
	
			CLoginDatabase CdatabasesData;
			retval = recv(wParam, (char*)&compareData, sizeof(compareData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"compareData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [COMPARE_NICKNAME_RQ] SUCCESS  \r\n");
			CompareNickname_RP compareNickname_RP;
			//여기 문제
			compareNickname_RP.compareNicknameSueccess = (CdatabasesData.CompareUserNickname(compareData.compareChar) != 0);
			retval = send(wParam, (char*)&compareNickname_RP, sizeof(compareNickname_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"compareId_RP send()");
				return;
			}
			DisplayText(L"COMPARE_NICKNAME_RQ 에서 [CompareNickname_RP] -> [SEND]\r\n");
		}
		break;
		//쪽지 데이터 받고 상대에게 보내주기
		case SINGLE_STR_DATA:
		{
			//쪽지 데이터 받기
			SingleData singleData;
			retval = recv(wParam, (char*)&singleData, sizeof(singleData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"singleData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [SINGLE_STR_DATA] SUCCESS  \r\n");

			//쪽지 데이터 출력
			DisplayText(L"접속한 아이디 : %s\r\n", singleData.id);
			DisplayText(L"보내는 메세지 : %s\r\n", singleData.str);

			//쪽지 받는 대상 찾고 내용 보내주기
			ClientInfoNode* ptr = CM_clientList.head;
			while (ptr)
			{
				if (wcscmp(ptr->id, singleData.id) == 0)
				{
					//보내는 클라 id 와 문자 내용 저장 후 전송
					SingleStrData singleStrData;
					wcscpy(singleStrData.SingleData.id, CM_clientList.FindId(wParam));
					wcscpy(singleStrData.SingleData.str, singleData.str);
					retval = send(ptr->sock, (char*)&singleStrData, sizeof(singleStrData), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"singleStrData send()");
						return;
					}
					DisplayText(L"SINGLE_STR_DATA 에서 [SingleStrData] -> [SEND]\r\n");
					//쪽지 전송 요청한 클라에게 성공 여부 알려주기
					SingleStrData_RP singleStrData_RP;
					singleStrData_RP.success = TRUE;
					retval = send(wParam, (char*)&singleStrData_RP, sizeof(singleStrData_RP), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"singleStrData_RP send()");
						return;
					}
					DisplayText(L"SINGLE_STR_DATA 에서 [SingleStrData_RP] -> [SEND]\r\n");
				}
				ptr = ptr->next;
			}
		}
		break;
		case CREATE_CHATTING_ROOM_RQ:
		{
			//채팅방 생성 요청 정보 받기
			CreateChattingRoomData createChattingRoomData;
			retval = recv(wParam, (char*)&createChattingRoomData, sizeof(createChattingRoomData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"createChattingRoomdata recv()");
				return;
			}
			DisplayText(L"[RECV] -> [CREATE_CHATTING_ROOM_RQ] SUCCESS  \r\n");
			//채팅창 관리 리스트에 채팅방 생성 정보 저장, 생성 요청 클라이언트 저장 
			CRM_chattingRoomList.AddClientManager(createChattingRoomData.name, createChattingRoomData.pw, 
												   createChattingRoomData.roomSize, wParam, CM_clientList.FindId(wParam));
			/*								WCHAR text[20];
											wsprintf(text, L"0 : %d", CRM_chattingRoomList.tail->roomUserNum);
											MessageBox(GetFocus(), text, L"채팅방 생성 1", MB_OK);*/
			//[다른 클라이언트에게 전송할 데이터] 채팅방 인원 수 정보, 방제 패킷에 저장
			ChattingRoomName_NF chattingRoomName_NF;
			WCHAR roomSizeInfo[4] = L"";
			swprintf(roomSizeInfo, 4, L"1/%d", createChattingRoomData.roomSize);
			wcscpy(chattingRoomName_NF.data.roomUserNumInfo, roomSizeInfo);
			wcscpy(chattingRoomName_NF.data.name, createChattingRoomData.name);
			//[현재 접속 클라이언트에게 전송할 데이터] 채팅방 인원 수 정보, 방제 패킷에 저장
			CreateChattingRoom_RP createChattingRoom_RP;
			wcscpy(createChattingRoom_RP.data.roomUserNumInfo, roomSizeInfo);
			wcscpy(createChattingRoom_RP.data.name, createChattingRoomData.name);
			createChattingRoom_RP.data.success = true;
			//비밀번호가 있을 시 TRUE로 설정
			if (wcscmp(createChattingRoomData.pw, L""))
			{
				chattingRoomName_NF.data.pwInfo = TRUE;
				createChattingRoom_RP.data.pwInfo = TRUE;
			}
			//모든 클라이언트에게 배포
			ClientInfoNode* ptr = CM_clientList.head;
			while (ptr)
			{
				//[접속해 있던 다른 클라이언트에게 배포]
				if (ptr->sock != wParam)
				{
					DisplayText(L" 채팅방 다른 클라에게 배포 중 \r\n");
					send(ptr->sock, (char*)&chattingRoomName_NF, sizeof(chattingRoomName_NF), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"chattingRoomName_NF send()");
						return;
					}
					ptr = ptr->next;
				}
				//[현재 접속 클라이언트에게 배포]
				else if (ptr->sock == wParam)
				{
					DisplayText(L" 채팅방 생성 클라에게 배포 중 \r\n");
					send(wParam, (char*)&createChattingRoom_RP, sizeof(createChattingRoom_RP), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"createChattingRoom_RP send()");
						return;
					}
					DisplayText(L"CREATE_CHATTING_ROOM_RQ 에서 [CreateChattingRoom_RP] -> [SEND]\r\n");
					ptr = ptr->next;
					continue;
				}
			}


		}
		break;
		//채팅룸 입장
		case CHATTING_ROOM_ENTER_RQ:
		{
			DisplayText(L"채팅방에 입장 요청 \r\n");
			
			//채팅방 입장 요청한 클라 정보 받기
			ChatEnterData chatEnterData;
			retval = recv(wParam, (char*)&chatEnterData, sizeof(chatEnterData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"chatEnterData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [CHATTING_ROOM_ENTER_RQ] SUCCESS  \r\n");
			
			//채팅방 위치 확인 후 저장 패킷
			ChattingRoomLimitNum_NF chattingRoomLimitNum_NF;

			//클라가 들어가려고 하는 채팅방 찾고 비밀번호 있는지 확인
			ClientManager* findRoomPtr = CRM_chattingRoomList.head;
			while (findRoomPtr)
			{
				//패킷으로 전송된 채팅방 이름과 같은 이름 찾으면
				if (wcscmp(findRoomPtr->roomName, chatEnterData.chattingRoomName) == 0)
				{
					DisplayText(L"채팅방에 발견 \r\n");
					//채팅방에 비밀번호가 존재 한다면 클라에 비밀번호 요청 
					if (wcscmp(findRoomPtr->roomPW, L"") != 0)
					{
						DisplayText(L"채팅방에 비밀번호 필요 클라에 요청 \r\n");
						ChatEnter_RP chatEnter_RP;
						wcscpy(chatEnter_RP.data.roomName, findRoomPtr->roomName);
						chatEnter_RP.data.havePW = TRUE;
						chatEnter_RP.data.success = FALSE;
						send(wParam, (char*)&chatEnter_RP, sizeof(chatEnter_RP), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"chatEnter_RP send()");
							return;
						}
						DisplayText(L"CHATTING_ROOM_ENTER_RQ 에서 [ChatEnter_RP] -> [SEND]\r\n");
						return;
					}
					//채팅방에 비밀번호가 존재 하지 않다면 채팅방에 클라 등록
					findRoomPtr->AddClientInfo(wParam, chatEnterData.loginID);
					findRoomPtr->roomUserNum = findRoomPtr->roomUserNum + 1;
					break;
				}
				else
				{
					DisplayText(L"채팅방 찾는 중 \r\n");
					findRoomPtr = findRoomPtr->next;
				}
				////////////////////
				chattingRoomLimitNum_NF.data.chatNumLocation = chattingRoomLimitNum_NF.data.chatNumLocation + 1;

			}
			/////////채팅방 입장 인원수 증가
			WCHAR roomSizeInfo[4] = L"";
			swprintf(roomSizeInfo, 4, L"%d/%d", findRoomPtr->roomUserNum, findRoomPtr->roomSize);
			wcscpy(chattingRoomLimitNum_NF.data.roomUserNum, roomSizeInfo);
			////////////////

			//채팅방 입장 성공 패킷 클라에게 전송
			ChatEnter_RP chatEnter_RP;
			wcscpy(chatEnter_RP.data.roomName, findRoomPtr->roomName);
			chatEnter_RP.data.success = TRUE;
			chatEnter_RP.data.havePW = FALSE;
			send(wParam, (char*)&chatEnter_RP, sizeof(chatEnter_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"chatEnter_RP send()");
				return;
			}
			DisplayText(L"CHATTING_ROOM_ENTER_RQ 에서 [ChatEnter_RP] -> [SEND]\r\n");
			DisplayText(L"채팅방 입장 완료\r\n");

			//채팅방에 입장한 클라 아이디 채팅방 접속 인원들에게 모두 알려주기
			OtherId_In_NF chatOtherId_In_NF;
			chatOtherId_In_NF.otherIdInHeader.header = CHAT_OTHER_ID_IN_NF;
			ClientInfoNode* ptr = findRoomPtr->head;
			while (ptr)
			{
				//////////////////	
				DisplayText(L" 채팅방에 접속 인원 알려주기 \r\n");
				send(ptr->sock, (char*)&chattingRoomLimitNum_NF, sizeof(chattingRoomLimitNum_NF), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"chattingRoomLimitNum_NF send()");
					return;
				}
				///////////////////
				DisplayText(L"채팅방에 입장한 클라에게 id 배포 중 \r\n");
				if (ptr->sock != wParam)
				{
					wcscpy(chatOtherId_In_NF.otherIdInData.otherID, chatEnterData.loginID);
					DisplayText(L" 채팅방에 접속한 클라 id 보내기 \r\n");
					send(ptr->sock, (char*)&chatOtherId_In_NF, sizeof(chatOtherId_In_NF), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"sendOtherId_NF send()");
						return;
					}
					wcscpy(chatOtherId_In_NF.otherIdInData.otherID, L"");
					wcscpy(chatOtherId_In_NF.otherIdInData.otherID, ptr->id);
					DisplayText(L" 채팅방에 접속해 있는 다른 클라 id 보내기 \r\n");
					send(wParam, (char*)&chatOtherId_In_NF, sizeof(chatOtherId_In_NF), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"chatOtherId_In_NF send()");
						return;
					}
					DisplayText(L"CHATTING_ROOM_ENTER_RQ 에서 [OtherId_In_NF] -> [SEND]\r\n");
					
				}
				ptr = ptr->next;
			}
		}
		break;
		//채팅방 입장 비밀번호 확인
		case CHECK_CHAT_PW_RQ:
		{
			CheckChatPwData_RQ checkChatPwData_RQ;
			retval = recv(wParam, (char*)&checkChatPwData_RQ, sizeof(CheckChatPwData_RQ), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"CheckChatPwData_RQ recv()");
				return;
			}
			DisplayText(L"[RECV] -> [CheckChatPwData_RQ SUCCESS]  \r\n");

			ClientManager* findRoomPtr = CRM_chattingRoomList.head;
			while (findRoomPtr)
			{
				if (wcscmp(findRoomPtr->roomName, checkChatPwData_RQ.roomName) == 0)
				{
					DisplayText(L"채팅방에 발견 \r\n");
					break;
				}
				else
				{
					DisplayText(L"채팅방 찾는 중 \r\n");
					findRoomPtr = findRoomPtr->next;
					continue;
				}
			}
			findRoomPtr->AddClientInfo(wParam, checkChatPwData_RQ.id);
			findRoomPtr->roomUserNum = findRoomPtr->roomUserNum + 1;

			CheckChatPw_RP checkChatPw_RP;
			if (wcscmp(findRoomPtr->roomPW, checkChatPwData_RQ.pw) == 0)
			{
				//채팅방 입장 성공 패킷 클라에게 전송 (클라에서 비밀번호 확인 다이얼로그 제거)
				checkChatPw_RP.success = TRUE;
				retval = send(wParam, (char*)&checkChatPw_RP, sizeof(CheckChatPw_RP), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"CheckChatPw_RP send()");
					return;
				}
				DisplayText(L"CHECK_CHAT_PW_RQ 에서 [CheckChatPw_RP] FALSE -> [SEND]\r\n");
				//여기문제
				ChatEnter_RP chatEnter_RP;
				wcscpy(chatEnter_RP.data.roomName, findRoomPtr->roomName);
				chatEnter_RP.data.havePW = TRUE;
				chatEnter_RP.data.success = TRUE;
				send(wParam, (char*)&chatEnter_RP, sizeof(chatEnter_RP), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"chatEnter_RP send()");
					return;
				}
				DisplayText(L"CHECK_CHAT_PW_RQ 에서 [ChatEnter_RP] -> [SEND]\r\n");
				DisplayText(L"채팅방 입장 완료\r\n");

				//채팅방에 입장한 클라 아이디 채팅방 접속 인원들에게 모두 알려주기
				OtherId_In_NF chatOtherId_In_NF;
				chatOtherId_In_NF.otherIdInHeader.header = CHAT_OTHER_ID_IN_NF;
				ClientInfoNode* ptr = findRoomPtr->head;
				while (ptr)
				{
					DisplayText(L"채팅방에 입장한 클라에게 id 배포 중 \r\n");
					if (ptr->sock != wParam)
					{
						wcscpy(chatOtherId_In_NF.otherIdInData.otherID, checkChatPwData_RQ.id);
						DisplayText(L" 채팅방에 접속한 클라 id 보내기 \r\n");
						send(ptr->sock, (char*)&chatOtherId_In_NF, sizeof(chatOtherId_In_NF), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"sendOtherId_NF send()");
							return;
						}
						memset(chatOtherId_In_NF.otherIdInData.otherID, 0, sizeof(chatOtherId_In_NF.otherIdInData.otherID));
						wcscpy(chatOtherId_In_NF.otherIdInData.otherID, ptr->id);
						DisplayText(L" 채팅방에 접속해 있는 다른 클라 id 보내기 \r\n");
						send(wParam, (char*)&chatOtherId_In_NF, sizeof(chatOtherId_In_NF), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"chatOtherId_In_NF send()");
							return;
						}
						DisplayText(L"CHECK_CHAT_PW_RQ 에서 [OtherId_In_NF] -> [SEND]\r\n");
						ptr = ptr->next;
					}
					else if (ptr->sock == wParam)
					{
						ptr = ptr->next;
						continue;
					}
				}
			}

			else
			{
				//생성 실패 보내기
				checkChatPw_RP.success = FALSE;
				retval = send(wParam, (char*)&checkChatPw_RP, sizeof(CheckChatPw_RP), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"CheckChatPw_RP send()");
					return;
				}
				DisplayText(L"CHECK_CHAT_PW_RQ 에서 [CheckChatPw_RP] FALSE -> [SEND]\r\n");
			}

		}
		break;
		case CHATTING_ROON_OUT_RQ:
		{
			//채팅방 나간 클라 정보 받기
			ChatOutData_RQ chatOutData_RQ;
			retval = recv(wParam, (char*)&chatOutData_RQ, sizeof(chatOutData_RQ), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"chatOutData_RQ recv()");
				return;
			}
			DisplayText(L"[RECV] -> [CHATTING_ROON_OUT_RQ SUCCESS]  \r\n");	
			
			//채팅방 위치 확인 후 저장 패킷
			ChattingRoomLimitNum_NF chattingRoomLimitNum_NF;

			//채팅방 찾기
			ClientManager* findRoomNamePtr = CRM_chattingRoomList.head;
			ClientManager* preFindRoomNamePtr = nullptr;
			while (findRoomNamePtr)
			{
				if (wcscmp(findRoomNamePtr->roomName, chatOutData_RQ.roomName) == 0)
				{
					break;
				}
				preFindRoomNamePtr = findRoomNamePtr;
				findRoomNamePtr	= findRoomNamePtr->next;
				chattingRoomLimitNum_NF.data.chatNumLocation = chattingRoomLimitNum_NF.data.chatNumLocation + 1;
			}
			//나간 인원 만큼 -1
			findRoomNamePtr->roomUserNum = findRoomNamePtr->roomUserNum - 1;
			
			/////////채팅방 입장 인원수 감소
			WCHAR roomSizeInfo[4] = L"";
			swprintf(roomSizeInfo, 4, L"%d/%d", findRoomNamePtr->roomUserNum, findRoomNamePtr->roomSize);
			wcscpy(chattingRoomLimitNum_NF.data.roomUserNum, roomSizeInfo);
			
			ClientInfoNode *sendAllSock = findRoomNamePtr->head;
			while (sendAllSock)
			{
				//////////////////	
				DisplayText(L" 채팅방 인원 변화 알려주기 \r\n");
				send(sendAllSock->sock, (char*)&chattingRoomLimitNum_NF, sizeof(chattingRoomLimitNum_NF), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"chattingRoomLimitNum_NF send()");
					return;
				}
				DisplayText(L"CHATTING_ROON_OUT_RQ 에서 [chattingRoomLimitNum_NF] -> [SEND]\r\n");
				sendAllSock = sendAllSock->next;
			}

			//채팅방에 저장된 클라 찾아 지우기
			ClientInfoNode* curr = findRoomNamePtr->head;
			ClientInfoNode* prev = nullptr;
			while (curr != nullptr)
			{
				if (curr->sock == wParam)
				{
					if (prev != nullptr)
					{
						if (curr == findRoomNamePtr->tail)
							findRoomNamePtr->tail = prev;
						prev->next = curr->next;
					}
					else
					{
						findRoomNamePtr->head = curr->next;
						findRoomNamePtr->tail = curr->next;
					}
					delete curr;
					break;
				}
				prev = curr;
				curr = curr->next;
			}
			//채팅방 나가기 요청한 클라에게 성공여부 전송
			ChatOut_RP chatOut_RP;
			chatOut_RP.success = TRUE;
			retval = send(wParam, (char*)&chatOut_RP, sizeof(ChatOut_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"chatOut_RP send()");
				return;
			}
			DisplayText(L"CHATTING_ROON_OUT_RQ 에서 [ChatOut_RP] -> [SEND]\r\n");
			//모든 클라에게 나간 클라 아이디 지우기 요청
			OtherId_Out_NF sendOtherIdOut_NF;
			sendOtherIdOut_NF.otherIdHeader.header = CHAT_OTHER_ID_OUT_NF;
			wcscpy(sendOtherIdOut_NF.otherIdOutData.otherID, chatOutData_RQ.id);

			ClientInfoNode *ptr = findRoomNamePtr->head;
			while (ptr)
			{
				DisplayText(L" 모든 클라에게 나간 클라 ID 지우기 요청 중.. \r\n");
				retval = send(ptr->sock, (char*)&sendOtherIdOut_NF, sizeof(sendOtherIdOut_NF), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"sendOtherIdOut_NF send()");
					return;
				}
				DisplayText(L"CHATTING_ROON_OUT_RQ 에서 [OtherId_Out_NF] -> [SEND]\r\n");
				ptr = ptr->next;
			}
			//채팅방에 아무도 안남아 있으면 모든 클라 채팅방 리스트 박스에서 제거
			if (findRoomNamePtr->roomUserNum == 0)
			{
				//채팅방 이름 패킷에 저장
				ChatRoomDelete_NF chatRoomDelete_NF;
				wcscpy(chatRoomDelete_NF.data.roomName, chatOutData_RQ.roomName);
				//모든 클라에 배포
				ptr = CM_clientList.head;
				while (ptr)
				{
					retval = send(ptr->sock, (char*)&chatRoomDelete_NF, sizeof(chatRoomDelete_NF), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"chatRoomDelete_NF send()");
						return;
					}
					DisplayText(L"CHATTING_ROON_OUT_RQ 에서 [ChatRoomDelete_NF] -> [SEND]\r\n");
					ptr = ptr->next;
				}
				CRM_chattingRoomList.RemoveClientManager(findRoomNamePtr, preFindRoomNamePtr);
			}
		}
		break;
		case CHAT_STR_DATA_RQ:
		{
			StrData_RQ strData_RQ;
			retval = recv(wParam, (char*)&strData_RQ, sizeof(strData_RQ), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"StrData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [CHAT_STR_DATA_RQ] SUCCESS  \r\n");
			addrLen = sizeof(clientAddr);
			getpeername(wParam, (SOCKADDR*)&clientAddr, &addrLen);
			DisplayText(L"[TCP /%S : %d] %s\r\n",
				inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), strData_RQ.str);

			ClientManager* findRoomNamePtr = CRM_chattingRoomList.head;

			while (findRoomNamePtr)
			{
				if (wcscmp(findRoomNamePtr->roomName, strData_RQ.chatRoomName) == 0)
				{
					break;
				}
				findRoomNamePtr = findRoomNamePtr->next;
			}
			ClientInfoNode* ptr = findRoomNamePtr->head;

			ChatStrData_RP chatStr;
			WCHAR findId[10] = L"";
			wcscpy(findId, findRoomNamePtr->FindId(wParam));
			DisplayText(L"%s \r\n", strData_RQ.str);
			while (ptr)
			{
				if(ptr->sock != wParam)
				{
					swprintf(chatStr.chatData.str, 40, L"%s : %s", findId, strData_RQ.str);
					retval = send(ptr->sock, (char*)&chatStr, sizeof(ChatStrData_RP), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"ChatStrData send()");
						return;
					}
				}
				else
				{
					swprintf(chatStr.chatData.str, 40, L"나 : %s", strData_RQ.str);
					retval = send(ptr->sock, (char*)&chatStr, sizeof(ChatStrData_RP), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"ChatStrData send()");
						return;
					}
				}
				DisplayText(L"CHATTING_ROON_OUT_RQ 에서 [ChatStrData_RP] -> [SEND]\r\n");
				ptr = ptr->next;
			}
		}
		break;
		}
		break;
	case FD_WRITE:
		DisplayText(L"FD_WRITE 발생\r\n");
		break;
	case FD_CLOSE:
		DisplayText(L"FD_CLOSE 발생\r\n");
		CM_clientList.RemoveClientInfo(wParam, SEND_OTHER_ID_OUT_NF);
		break;
	}
}



void DisplayText(LPWSTR fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	WCHAR cbuf[BUFSIZE + 256];
	vswprintf(cbuf, BUFSIZE + 256, fmt, arg);

	int nLength = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

	va_end(arg);
}

void err_quit(LPWSTR msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(LPWSTR msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	DisplayText(L"[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	DisplayText(L"[오류] %s", (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

