// WSAAsyncSelect_Server.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


#define MAX_LOADSTRING 100
//server data 
#define SERVERPORT 9000
#define BUFSIZE	   512
#define WM_SOCKET (WM_USER+1)

// ���� ����
HINSTANCE	hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR		szTitle[MAX_LOADSTRING];              // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR		szWindowClass[MAX_LOADSTRING];        // �⺻ â Ŭ���� �̸��Դϴ�.
HWND		hEdit;
// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//���� ó�� �Լ�
void				InitServer(HWND hWnd);
void				ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
//error ��� �Լ�
void DisplayText(LPWSTR fmt, ...);
void err_quit(LPWSTR msg);
void err_display(LPWSTR msg);
void err_display(int errcode);
//Ŭ���̾�Ʈ ���� Ŭ����
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

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WSAASYNCSELECT_SERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WSAASYNCSELECT_SERVER));

	MSG msg;
	_wsetlocale(LC_ALL, L"korean");
	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
		//���� �ʱ�ȭ �� ���� ���� �� listen() ��������
		InitServer(hWnd);
		return 0;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_SOCKET:
		//���� ���� �޼��� ó��
		ProcessSocketMessage(hWnd, message, wParam, lParam);
		return 0;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
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
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
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


// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
//  �Լ�: InitServer(HWND hWnd)
//
//  ����:  ���� ���� ������ ���õ� ����
//
//  ����:  ���� ���� �� �ʱ�ȭ, 
//        ���� ���� �� bind,listen ����, 
//		  WSAAsyncSelect() ����Ͽ� listen_sock ���
void InitServer(HWND hWnd)
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	return;
	//socket ���� AF_INET, SOCK_STREAM
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
		DisplayText(L"���� �߻� \r\n");
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
		DisplayText(L"[TCP ����] : Client ���� \r\n IP �ּ� : %S, ��Ʈ ��ȣ : %d\r\n",
			inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
		if (WSAAsyncSelect(clientSock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
		{
			err_display(L"WSAAsyncSelect(), clientSock");
		}
		break;
	}
	case FD_READ:
		//������ �ޱ�
		DisplayText(L"Header �ޱ� \r\n");
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
			//ȸ�� ���� ������ �ޱ�
			DisplayText(L"SIGNUP_RQ �ޱ� \r\n");
			SignUpData signupData;
			retval = recv(wParam, (char*)&signupData, sizeof(signupData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"signupData recv()");
				err_display(WSAGetLastError());
				return;
			}
			DisplayText(L"[RECV] -> [SIGNUP_RQ] SUCCESS  \r\n");
			//ȸ�� ���� ���� ���� Ȯ��
			SignUp_RP signup_RP;
			CLoginDatabase CdatabasesData;
			signup_RP.signupSucess = (CdatabasesData.CreateUserID(signupData.createID, signupData.createPW, signupData.createNickname, signupData.createQuest) != 0);
			//��� ����
			retval = send(wParam, (char*)&signup_RP, sizeof(signup_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"signup_RP send()");
				return;
			}
			DisplayText(L"SIGNUP_RQ ���� [SignUp_RP] -> [SEND]\r\n");
		}
		break;
		case LOGIN_RQ:
		{
			//�α��� ������ �ޱ�
			DisplayText(L"LOGIN_RQ �ޱ� \r\n");
			LoginData loginData;
			retval = recv(wParam, (char*)&loginData, sizeof(loginData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"loginData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [LOGIN_RQ] SUCCESS  \r\n");
			//�α��� ���� ���� ��Ŷ ����
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
			DisplayText(L"LOGIN_RQ ���� [Login_RP] -> [SEND]\r\n");
			//�α��� ���� �� �α��� �� Ŭ�� �г��� �ٸ� Ŭ�󿡰� �˷��ֱ�
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
					DisplayText(L"Ŭ�󿡰� id ���� �� \r\n");
					if (ptr->sock != wParam)
					{
						wcscpy(sendOtherId_In_NF.otherIdInData.otherID, nickname);
						DisplayText(L"�ٸ� Ŭ�󿡰� ���� ������ id ������ %s \r\n", sendOtherId_In_NF.otherIdInData.otherID);
						send(ptr->sock, (char*)&sendOtherId_In_NF, sizeof(sendOtherId_In_NF), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"sendOtherId_NF send()");
							return;
						}
						DisplayText(L"LOGIN_RQ ���� [OtherId_In_NF] -> [SEND]\r\n");
						memset(sendOtherId_In_NF.otherIdInData.otherID, 0, sizeof(sendOtherId_In_NF.otherIdInData.otherID));
						wcscpy(sendOtherId_In_NF.otherIdInData.otherID, ptr->id);
						DisplayText(L"���� Ŭ�󿡰� ������ �����ߴ� id ������ %s \r\n", sendOtherId_In_NF.otherIdInData.otherID);
						send(wParam, (char*)&sendOtherId_In_NF, sizeof(sendOtherId_In_NF), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"sendOtherId_NF send()");
							return;
						}
						DisplayText(L"LOGIN_RQ ���� [OtherId_In_NF] -> [SEND]\r\n");
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
			DisplayText(L"SEARCH_PW_RQ ���� SearchPW_RP -> [SEND]\r\n");
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
			DisplayText(L"COMPARE_ID_RQ ���� [CompareID_RP] -> [SEND]\r\n");
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
			//���� ����
			compareNickname_RP.compareNicknameSueccess = (CdatabasesData.CompareUserNickname(compareData.compareChar) != 0);
			retval = send(wParam, (char*)&compareNickname_RP, sizeof(compareNickname_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"compareId_RP send()");
				return;
			}
			DisplayText(L"COMPARE_NICKNAME_RQ ���� [CompareNickname_RP] -> [SEND]\r\n");
		}
		break;
		//���� ������ �ް� ��뿡�� �����ֱ�
		case SINGLE_STR_DATA:
		{
			//���� ������ �ޱ�
			SingleData singleData;
			retval = recv(wParam, (char*)&singleData, sizeof(singleData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"singleData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [SINGLE_STR_DATA] SUCCESS  \r\n");

			//���� ������ ���
			DisplayText(L"������ ���̵� : %s\r\n", singleData.id);
			DisplayText(L"������ �޼��� : %s\r\n", singleData.str);

			//���� �޴� ��� ã�� ���� �����ֱ�
			ClientInfoNode* ptr = CM_clientList.head;
			while (ptr)
			{
				if (wcscmp(ptr->id, singleData.id) == 0)
				{
					//������ Ŭ�� id �� ���� ���� ���� �� ����
					SingleStrData singleStrData;
					wcscpy(singleStrData.SingleData.id, CM_clientList.FindId(wParam));
					wcscpy(singleStrData.SingleData.str, singleData.str);
					retval = send(ptr->sock, (char*)&singleStrData, sizeof(singleStrData), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"singleStrData send()");
						return;
					}
					DisplayText(L"SINGLE_STR_DATA ���� [SingleStrData] -> [SEND]\r\n");
					//���� ���� ��û�� Ŭ�󿡰� ���� ���� �˷��ֱ�
					SingleStrData_RP singleStrData_RP;
					singleStrData_RP.success = TRUE;
					retval = send(wParam, (char*)&singleStrData_RP, sizeof(singleStrData_RP), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"singleStrData_RP send()");
						return;
					}
					DisplayText(L"SINGLE_STR_DATA ���� [SingleStrData_RP] -> [SEND]\r\n");
				}
				ptr = ptr->next;
			}
		}
		break;
		case CREATE_CHATTING_ROOM_RQ:
		{
			//ä�ù� ���� ��û ���� �ޱ�
			CreateChattingRoomData createChattingRoomData;
			retval = recv(wParam, (char*)&createChattingRoomData, sizeof(createChattingRoomData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"createChattingRoomdata recv()");
				return;
			}
			DisplayText(L"[RECV] -> [CREATE_CHATTING_ROOM_RQ] SUCCESS  \r\n");
			//ä��â ���� ����Ʈ�� ä�ù� ���� ���� ����, ���� ��û Ŭ���̾�Ʈ ���� 
			CRM_chattingRoomList.AddClientManager(createChattingRoomData.name, createChattingRoomData.pw, 
												   createChattingRoomData.roomSize, wParam, CM_clientList.FindId(wParam));
			/*								WCHAR text[20];
											wsprintf(text, L"0 : %d", CRM_chattingRoomList.tail->roomUserNum);
											MessageBox(GetFocus(), text, L"ä�ù� ���� 1", MB_OK);*/
			//[�ٸ� Ŭ���̾�Ʈ���� ������ ������] ä�ù� �ο� �� ����, ���� ��Ŷ�� ����
			ChattingRoomName_NF chattingRoomName_NF;
			WCHAR roomSizeInfo[4] = L"";
			swprintf(roomSizeInfo, 4, L"1/%d", createChattingRoomData.roomSize);
			wcscpy(chattingRoomName_NF.data.roomUserNumInfo, roomSizeInfo);
			wcscpy(chattingRoomName_NF.data.name, createChattingRoomData.name);
			//[���� ���� Ŭ���̾�Ʈ���� ������ ������] ä�ù� �ο� �� ����, ���� ��Ŷ�� ����
			CreateChattingRoom_RP createChattingRoom_RP;
			wcscpy(createChattingRoom_RP.data.roomUserNumInfo, roomSizeInfo);
			wcscpy(createChattingRoom_RP.data.name, createChattingRoomData.name);
			createChattingRoom_RP.data.success = true;
			//��й�ȣ�� ���� �� TRUE�� ����
			if (wcscmp(createChattingRoomData.pw, L""))
			{
				chattingRoomName_NF.data.pwInfo = TRUE;
				createChattingRoom_RP.data.pwInfo = TRUE;
			}
			//��� Ŭ���̾�Ʈ���� ����
			ClientInfoNode* ptr = CM_clientList.head;
			while (ptr)
			{
				//[������ �ִ� �ٸ� Ŭ���̾�Ʈ���� ����]
				if (ptr->sock != wParam)
				{
					DisplayText(L" ä�ù� �ٸ� Ŭ�󿡰� ���� �� \r\n");
					send(ptr->sock, (char*)&chattingRoomName_NF, sizeof(chattingRoomName_NF), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"chattingRoomName_NF send()");
						return;
					}
					ptr = ptr->next;
				}
				//[���� ���� Ŭ���̾�Ʈ���� ����]
				else if (ptr->sock == wParam)
				{
					DisplayText(L" ä�ù� ���� Ŭ�󿡰� ���� �� \r\n");
					send(wParam, (char*)&createChattingRoom_RP, sizeof(createChattingRoom_RP), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"createChattingRoom_RP send()");
						return;
					}
					DisplayText(L"CREATE_CHATTING_ROOM_RQ ���� [CreateChattingRoom_RP] -> [SEND]\r\n");
					ptr = ptr->next;
					continue;
				}
			}


		}
		break;
		//ä�÷� ����
		case CHATTING_ROOM_ENTER_RQ:
		{
			DisplayText(L"ä�ù濡 ���� ��û \r\n");
			
			//ä�ù� ���� ��û�� Ŭ�� ���� �ޱ�
			ChatEnterData chatEnterData;
			retval = recv(wParam, (char*)&chatEnterData, sizeof(chatEnterData), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"chatEnterData recv()");
				return;
			}
			DisplayText(L"[RECV] -> [CHATTING_ROOM_ENTER_RQ] SUCCESS  \r\n");
			
			//ä�ù� ��ġ Ȯ�� �� ���� ��Ŷ
			ChattingRoomLimitNum_NF chattingRoomLimitNum_NF;

			//Ŭ�� ������ �ϴ� ä�ù� ã�� ��й�ȣ �ִ��� Ȯ��
			ClientManager* findRoomPtr = CRM_chattingRoomList.head;
			while (findRoomPtr)
			{
				//��Ŷ���� ���۵� ä�ù� �̸��� ���� �̸� ã����
				if (wcscmp(findRoomPtr->roomName, chatEnterData.chattingRoomName) == 0)
				{
					DisplayText(L"ä�ù濡 �߰� \r\n");
					//ä�ù濡 ��й�ȣ�� ���� �Ѵٸ� Ŭ�� ��й�ȣ ��û 
					if (wcscmp(findRoomPtr->roomPW, L"") != 0)
					{
						DisplayText(L"ä�ù濡 ��й�ȣ �ʿ� Ŭ�� ��û \r\n");
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
						DisplayText(L"CHATTING_ROOM_ENTER_RQ ���� [ChatEnter_RP] -> [SEND]\r\n");
						return;
					}
					//ä�ù濡 ��й�ȣ�� ���� ���� �ʴٸ� ä�ù濡 Ŭ�� ���
					findRoomPtr->AddClientInfo(wParam, chatEnterData.loginID);
					findRoomPtr->roomUserNum = findRoomPtr->roomUserNum + 1;
					break;
				}
				else
				{
					DisplayText(L"ä�ù� ã�� �� \r\n");
					findRoomPtr = findRoomPtr->next;
				}
				////////////////////
				chattingRoomLimitNum_NF.data.chatNumLocation = chattingRoomLimitNum_NF.data.chatNumLocation + 1;

			}
			/////////ä�ù� ���� �ο��� ����
			WCHAR roomSizeInfo[4] = L"";
			swprintf(roomSizeInfo, 4, L"%d/%d", findRoomPtr->roomUserNum, findRoomPtr->roomSize);
			wcscpy(chattingRoomLimitNum_NF.data.roomUserNum, roomSizeInfo);
			////////////////

			//ä�ù� ���� ���� ��Ŷ Ŭ�󿡰� ����
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
			DisplayText(L"CHATTING_ROOM_ENTER_RQ ���� [ChatEnter_RP] -> [SEND]\r\n");
			DisplayText(L"ä�ù� ���� �Ϸ�\r\n");

			//ä�ù濡 ������ Ŭ�� ���̵� ä�ù� ���� �ο��鿡�� ��� �˷��ֱ�
			OtherId_In_NF chatOtherId_In_NF;
			chatOtherId_In_NF.otherIdInHeader.header = CHAT_OTHER_ID_IN_NF;
			ClientInfoNode* ptr = findRoomPtr->head;
			while (ptr)
			{
				//////////////////	
				DisplayText(L" ä�ù濡 ���� �ο� �˷��ֱ� \r\n");
				send(ptr->sock, (char*)&chattingRoomLimitNum_NF, sizeof(chattingRoomLimitNum_NF), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"chattingRoomLimitNum_NF send()");
					return;
				}
				///////////////////
				DisplayText(L"ä�ù濡 ������ Ŭ�󿡰� id ���� �� \r\n");
				if (ptr->sock != wParam)
				{
					wcscpy(chatOtherId_In_NF.otherIdInData.otherID, chatEnterData.loginID);
					DisplayText(L" ä�ù濡 ������ Ŭ�� id ������ \r\n");
					send(ptr->sock, (char*)&chatOtherId_In_NF, sizeof(chatOtherId_In_NF), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"sendOtherId_NF send()");
						return;
					}
					wcscpy(chatOtherId_In_NF.otherIdInData.otherID, L"");
					wcscpy(chatOtherId_In_NF.otherIdInData.otherID, ptr->id);
					DisplayText(L" ä�ù濡 ������ �ִ� �ٸ� Ŭ�� id ������ \r\n");
					send(wParam, (char*)&chatOtherId_In_NF, sizeof(chatOtherId_In_NF), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"chatOtherId_In_NF send()");
						return;
					}
					DisplayText(L"CHATTING_ROOM_ENTER_RQ ���� [OtherId_In_NF] -> [SEND]\r\n");
					
				}
				ptr = ptr->next;
			}
		}
		break;
		//ä�ù� ���� ��й�ȣ Ȯ��
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
					DisplayText(L"ä�ù濡 �߰� \r\n");
					break;
				}
				else
				{
					DisplayText(L"ä�ù� ã�� �� \r\n");
					findRoomPtr = findRoomPtr->next;
					continue;
				}
			}
			findRoomPtr->AddClientInfo(wParam, checkChatPwData_RQ.id);
			findRoomPtr->roomUserNum = findRoomPtr->roomUserNum + 1;

			CheckChatPw_RP checkChatPw_RP;
			if (wcscmp(findRoomPtr->roomPW, checkChatPwData_RQ.pw) == 0)
			{
				//ä�ù� ���� ���� ��Ŷ Ŭ�󿡰� ���� (Ŭ�󿡼� ��й�ȣ Ȯ�� ���̾�α� ����)
				checkChatPw_RP.success = TRUE;
				retval = send(wParam, (char*)&checkChatPw_RP, sizeof(CheckChatPw_RP), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"CheckChatPw_RP send()");
					return;
				}
				DisplayText(L"CHECK_CHAT_PW_RQ ���� [CheckChatPw_RP] FALSE -> [SEND]\r\n");
				//���⹮��
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
				DisplayText(L"CHECK_CHAT_PW_RQ ���� [ChatEnter_RP] -> [SEND]\r\n");
				DisplayText(L"ä�ù� ���� �Ϸ�\r\n");

				//ä�ù濡 ������ Ŭ�� ���̵� ä�ù� ���� �ο��鿡�� ��� �˷��ֱ�
				OtherId_In_NF chatOtherId_In_NF;
				chatOtherId_In_NF.otherIdInHeader.header = CHAT_OTHER_ID_IN_NF;
				ClientInfoNode* ptr = findRoomPtr->head;
				while (ptr)
				{
					DisplayText(L"ä�ù濡 ������ Ŭ�󿡰� id ���� �� \r\n");
					if (ptr->sock != wParam)
					{
						wcscpy(chatOtherId_In_NF.otherIdInData.otherID, checkChatPwData_RQ.id);
						DisplayText(L" ä�ù濡 ������ Ŭ�� id ������ \r\n");
						send(ptr->sock, (char*)&chatOtherId_In_NF, sizeof(chatOtherId_In_NF), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"sendOtherId_NF send()");
							return;
						}
						memset(chatOtherId_In_NF.otherIdInData.otherID, 0, sizeof(chatOtherId_In_NF.otherIdInData.otherID));
						wcscpy(chatOtherId_In_NF.otherIdInData.otherID, ptr->id);
						DisplayText(L" ä�ù濡 ������ �ִ� �ٸ� Ŭ�� id ������ \r\n");
						send(wParam, (char*)&chatOtherId_In_NF, sizeof(chatOtherId_In_NF), 0);
						if (retval == SOCKET_ERROR)
						{
							err_display(L"chatOtherId_In_NF send()");
							return;
						}
						DisplayText(L"CHECK_CHAT_PW_RQ ���� [OtherId_In_NF] -> [SEND]\r\n");
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
				//���� ���� ������
				checkChatPw_RP.success = FALSE;
				retval = send(wParam, (char*)&checkChatPw_RP, sizeof(CheckChatPw_RP), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"CheckChatPw_RP send()");
					return;
				}
				DisplayText(L"CHECK_CHAT_PW_RQ ���� [CheckChatPw_RP] FALSE -> [SEND]\r\n");
			}

		}
		break;
		case CHATTING_ROON_OUT_RQ:
		{
			//ä�ù� ���� Ŭ�� ���� �ޱ�
			ChatOutData_RQ chatOutData_RQ;
			retval = recv(wParam, (char*)&chatOutData_RQ, sizeof(chatOutData_RQ), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"chatOutData_RQ recv()");
				return;
			}
			DisplayText(L"[RECV] -> [CHATTING_ROON_OUT_RQ SUCCESS]  \r\n");	
			
			//ä�ù� ��ġ Ȯ�� �� ���� ��Ŷ
			ChattingRoomLimitNum_NF chattingRoomLimitNum_NF;

			//ä�ù� ã��
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
			//���� �ο� ��ŭ -1
			findRoomNamePtr->roomUserNum = findRoomNamePtr->roomUserNum - 1;
			
			/////////ä�ù� ���� �ο��� ����
			WCHAR roomSizeInfo[4] = L"";
			swprintf(roomSizeInfo, 4, L"%d/%d", findRoomNamePtr->roomUserNum, findRoomNamePtr->roomSize);
			wcscpy(chattingRoomLimitNum_NF.data.roomUserNum, roomSizeInfo);
			
			ClientInfoNode *sendAllSock = findRoomNamePtr->head;
			while (sendAllSock)
			{
				//////////////////	
				DisplayText(L" ä�ù� �ο� ��ȭ �˷��ֱ� \r\n");
				send(sendAllSock->sock, (char*)&chattingRoomLimitNum_NF, sizeof(chattingRoomLimitNum_NF), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"chattingRoomLimitNum_NF send()");
					return;
				}
				DisplayText(L"CHATTING_ROON_OUT_RQ ���� [chattingRoomLimitNum_NF] -> [SEND]\r\n");
				sendAllSock = sendAllSock->next;
			}

			//ä�ù濡 ����� Ŭ�� ã�� �����
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
			//ä�ù� ������ ��û�� Ŭ�󿡰� �������� ����
			ChatOut_RP chatOut_RP;
			chatOut_RP.success = TRUE;
			retval = send(wParam, (char*)&chatOut_RP, sizeof(ChatOut_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display(L"chatOut_RP send()");
				return;
			}
			DisplayText(L"CHATTING_ROON_OUT_RQ ���� [ChatOut_RP] -> [SEND]\r\n");
			//��� Ŭ�󿡰� ���� Ŭ�� ���̵� ����� ��û
			OtherId_Out_NF sendOtherIdOut_NF;
			sendOtherIdOut_NF.otherIdHeader.header = CHAT_OTHER_ID_OUT_NF;
			wcscpy(sendOtherIdOut_NF.otherIdOutData.otherID, chatOutData_RQ.id);

			ClientInfoNode *ptr = findRoomNamePtr->head;
			while (ptr)
			{
				DisplayText(L" ��� Ŭ�󿡰� ���� Ŭ�� ID ����� ��û ��.. \r\n");
				retval = send(ptr->sock, (char*)&sendOtherIdOut_NF, sizeof(sendOtherIdOut_NF), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display(L"sendOtherIdOut_NF send()");
					return;
				}
				DisplayText(L"CHATTING_ROON_OUT_RQ ���� [OtherId_Out_NF] -> [SEND]\r\n");
				ptr = ptr->next;
			}
			//ä�ù濡 �ƹ��� �ȳ��� ������ ��� Ŭ�� ä�ù� ����Ʈ �ڽ����� ����
			if (findRoomNamePtr->roomUserNum == 0)
			{
				//ä�ù� �̸� ��Ŷ�� ����
				ChatRoomDelete_NF chatRoomDelete_NF;
				wcscpy(chatRoomDelete_NF.data.roomName, chatOutData_RQ.roomName);
				//��� Ŭ�� ����
				ptr = CM_clientList.head;
				while (ptr)
				{
					retval = send(ptr->sock, (char*)&chatRoomDelete_NF, sizeof(chatRoomDelete_NF), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"chatRoomDelete_NF send()");
						return;
					}
					DisplayText(L"CHATTING_ROON_OUT_RQ ���� [ChatRoomDelete_NF] -> [SEND]\r\n");
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
					swprintf(chatStr.chatData.str, 40, L"�� : %s", strData_RQ.str);
					retval = send(ptr->sock, (char*)&chatStr, sizeof(ChatStrData_RP), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display(L"ChatStrData send()");
						return;
					}
				}
				DisplayText(L"CHATTING_ROON_OUT_RQ ���� [ChatStrData_RP] -> [SEND]\r\n");
				ptr = ptr->next;
			}
		}
		break;
		}
		break;
	case FD_WRITE:
		DisplayText(L"FD_WRITE �߻�\r\n");
		break;
	case FD_CLOSE:
		DisplayText(L"FD_CLOSE �߻�\r\n");
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
	DisplayText(L"[����] %s", (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

