// chatting_client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "chatting_client.h"

#define MAX_LOADSTRING 100

WCHAR myId[10];
WCHAR myChatRoomName[20];
enum EditName {
	ID_LOGIN_SIGNUP_BUTTON = 200,
	ID_LOGIN_SEARCH_PW_BUTTON,
	ID_LOGIN_ID_EDIT,
	ID_LOGIN_PASSWORD_EDIT,
	ID_LOGIN_BUTTON,
	ID_IDLIST_MYID_EDIT,
	ID_IDLIST_OTHERID_LISTBOX,
	ID_IDLIST_WINDOW_BUTTON,
	ID_CHATTING_ROOM_NAME_STATIC,
	ID_CHATTING_ROOM_USERNUM_STATIC,
	ID_CHATTING_ROOM_SECRET_LISTBOX,
	ID_CHATTING_ROOM_NAME_LISTBOX,
	ID_CHATTING_ROOM_USERNUM_LISTBOX,
	ID_CHATTING_ROOM_WINDOW_BUTTON,
	ID_CHATTING_ROOM_CREATE_ROOM_BUTTON,
	ID_CHATTING_ROOM_ENTER_ROOM_BUTTON,
	ID_CHATTING_WRITE_EDIT,
	ID_CHATTING_READ_LISTBOX,
	ID_CHATTING_SEND_BUTTON
};
/////////////// server data //////////////////
#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define BUFSIZE		512
#define WM_SOCKET (WM_USER+1)
/////////////////////////////////////////////


// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

HBITMAP hBitmap;
HBRUSH  hBrush_Control;

HWND hLoginSignUpButton;
HWND hLoginSearchPwButton;
HWND hLoginIdEdit;
HWND hLoginPwEdit;
HWND hLoginQuestEdit;
HWND hLoginButton;
HWND hChattingRoomName;
HWND hChattingRoomUserNum;
HWND hChattingRoomSecretListbox;
HWND hChattingRoomNameListbox;
HWND hChattingRoomUserNumListbox;
HWND hIdListWindowButton;
HWND hChattingRoomWindowButton;
HWND hChattingRoomCreateRoomButton;
HWND hChattingRoomEnterRoomButton;

HWND hIdListMyidEdit;
HWND hIdListOtheridListbox;

HWND hChatWriteEdit;
HWND hChatReadListbox;
HWND hChatSendButton;
//���̾� �α� �ڵ�
HWND hDlg_CreateIdProc;
HWND hDlg_SearchPwProc;
HWND hDlg_SendMessageProc;
HWND hDlg_RecvMessageProc;
HWND hDlg_ChatMessageProc;
HWND hDlg_CreateChattingRoomProc;
HWND hDlg_CheckChatPwProc;

WCHAR g_str[BUFSIZE];
SOCKET g_serverSock;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
BOOL CALLBACK		Dlg_CreateIdProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK		Dlg_SearchPwProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK		Dlg_SendMessageProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK		Dlg_RecvMessageProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK		Dlg_CheckChatPwProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK		Dlg_CreateChattingRoomProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
////////////////ȭ��  �Լ�////////////////////
void CreateLoginWindow(HWND hWnd, LPARAM lParam);
void CreateIdListWindow(HWND hWnd);
void CreateChattingWindow(HWND hWnd);
////////////////����  �Լ�////////////////////
void InitClientServer(HWND hWnd);
//////////////error ��� �Լ�/////////////////
void DisplayText(LPWSTR fmt, ...);
//void err_quit(LPWSTR msg);
//void err_display(LPWSTR msg);
//void err_display(int errcode);
/////////////////////////////////////////////

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
	LoadStringW(hInstance, IDC_CHATTING_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHATTING_CLIENT));

	MSG msg;
	_wsetlocale(LC_ALL, L"korean");
	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg) || !IsDialogMessage(hDlg_RecvMessageProc, &msg))
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHATTING_CLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CHATTING_CLIENT);
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
		200, 200, 500, 500, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK Dlg_CreateChattingRoomProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	hDlg_CreateChattingRoomProc = hDlg;
	HWND hOnPwRadio = GetDlgItem(hDlg_CreateChattingRoomProc, IDC_CREATE_CHATTING_ROOM_PW_EDIT);
	WCHAR roomName[20] = L"";
	static int num;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		num = 4;
		CheckRadioButton(hDlg, IDC_ON_PW_RADIO, IDC_OFF_PW_RADIO, IDC_OFF_PW_RADIO);
		CheckRadioButton(hDlg, IDC_FOUR_RADIO, IDC_NO_LIMIT_RADIO, IDC_FOUR_RADIO);
		SetDlgItemText(hDlg, IDC_CREATE_PW_EDIT, L"");
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_ON_PW_RADIO:
			EnableWindow(hOnPwRadio, TRUE);
			break;
		case IDC_OFF_PW_RADIO:
			SetDlgItemText(hDlg, IDC_CREATE_PW_EDIT, L"");
			EnableWindow(hOnPwRadio, FALSE);
			break;
		case IDC_FOUR_RADIO:
			num = 4;
			break;
		case IDC_EIGHT_RADIO:
			num = 8;
			break;
		case IDC_NO_LIMIT_RADIO:
			num = 0;
			break;
		case IDOK:
			GetDlgItemText(hDlg, IDC_CREATE_CHATTING_ROOM_NAME_EDIT, roomName, sizeof(roomName));
			if (wcscmp(roomName, L"") != 0)
			{
				wcscpy(myChatRoomName, roomName);
				CreateChattingRoom(g_serverSock, num, hDlg_CreateChattingRoomProc);
			}
			else
			{
				MessageBox(hDlg_CreateChattingRoomProc, L"�� ������ ���� �Ͽ����ϴ�.", L"������ �Է��ϼ���", 0);
			}
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK Dlg_CreateIdProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	hDlg_CreateIdProc = hDlg;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCHECK:
			SendIdCheck_RQ(g_serverSock, hDlg);
			break;
		case NICKNAMECHECK:
			SendNicknameCheck_RQ(g_serverSock, hDlg);
			break;
		case IDOK:
			SendSignUp_RQ(g_serverSock, hDlg);
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK Dlg_SearchPwProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	hDlg_SearchPwProc = hDlg;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDSEARCH:
			SendSearchPW_RQ(g_serverSock, hDlg);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK Dlg_SendMessageProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	hDlg_SendMessageProc = hDlg;
	WCHAR recvId[10] = L"";
	switch (iMsg)
	{
	case WM_INITDIALOG:

		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg_SendMessageProc, IDC_SEND_ID_EDIT, recvId, 10);
			SendSingleStr(g_serverSock, hDlg_SendMessageProc, recvId);
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK Dlg_RecvMessageProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	hDlg_RecvMessageProc = hDlg;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDRESEND:
		{
			WCHAR recvId[10] = L"";
			hDlg_SendMessageProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SEND_MESSAGE), hDlg_SendMessageProc, Dlg_SendMessageProc);
			ShowWindow(hDlg_SendMessageProc, SW_SHOW);
			GetDlgItemText(hDlg_RecvMessageProc, IDC_RECV_ID_EDIT, recvId, 10);
			SendDlgItemMessage(hDlg_SendMessageProc, IDC_SEND_ID_EDIT, EM_REPLACESEL, FALSE, (LPARAM)recvId);
			DestroyWindow(hDlg_RecvMessageProc);
			hDlg_RecvMessageProc = NULL;
			return TRUE;
		}
		case IDOK:
		{
			DestroyWindow(hDlg_RecvMessageProc);
			hDlg_RecvMessageProc = NULL;
			return TRUE;
		}
		case IDCANCEL:
			DestroyWindow(hDlg_RecvMessageProc);
			hDlg_RecvMessageProc = NULL;
			return TRUE;
		}
		break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK Dlg_ChatMessageProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	hDlg_ChatMessageProc = hDlg;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case  WM_CTLCOLORLISTBOX:
		if((HWND)lParam == GetDlgItem(hDlg, IDC_CHAT_RECV_LISTBOX))
		{
			SetBkColor((HDC)wParam, RGB(234, 234, 234));
			SetTextColor((HDC)wParam, RGB(29, 219, 22));
			return (BOOL)(hBrush_Control = CreateSolidBrush(RGB(255, 255, 255)));
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDSEND:
		{
			SendChatStr(g_serverSock, hDlg_ChatMessageProc);
			SetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_SEND_EDIT, L"");
			break;
		}
		case IDCANCEL:
			SendChatRoomOut(g_serverSock, hDlg_ChatMessageProc);
			DestroyWindow(hDlg_ChatMessageProc);
			hDlg_ChatMessageProc = NULL;
			//EndDialog(hDlg, 0);
		}
		break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK Dlg_CheckChatPwProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	hDlg_CheckChatPwProc = hDlg;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CheckChatPW_RQ(g_serverSock, hDlg_CheckChatPwProc, myId);
			break;
		}
		case IDCANCEL:
			DestroyWindow(hDlg_CheckChatPwProc);
			hDlg_CheckChatPwProc = NULL;
		}
		break;
	}
	return 0;
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

	//Login_RQ login;
	switch (message)
	{
	case WM_CREATE:
		CreateLoginWindow(hWnd, lParam);
		InitClientServer(hWnd);
		//CreateChattingWindow(hWnd);
		//CreateIdListWindow(hWnd);
		hBrush_Control = CreateSolidBrush(RGB(255, 255, 255));   //��� �귯�� ����.
		return 0;
		//�ϴܽ��
	case WM_SETFOCUS:
		SetFocus(hChatWriteEdit);
		return 0;
	case WM_SOCKET:
		ProcessSocketMessage(hWnd, message, wParam, lParam);
		return 0;
	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)wParam, RGB(255, 255, 255));
		return (LRESULT)hBrush_Control;
	/*case  WM_CTLCOLORLISTBOX:
		SetBkColor((HDC)wParam, RGB(255, 0, 0));
		SetTextColor((HDC)wParam, RGB(255, 255, 255));
		return (BOOL)(hBrush_Control = CreateSolidBrush(RGB(255, 0, 0)));*/
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case ID_LOGIN_SIGNUP_BUTTON:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CREATE_ID_DIALOG), hWnd, Dlg_CreateIdProc);
			break;
		case ID_LOGIN_SEARCH_PW_BUTTON:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SEARCH_PW_DIALOG), hWnd, Dlg_SearchPwProc);
			break;
		case ID_LOGIN_BUTTON:
			SendLogin_RQ(g_serverSock, hLoginIdEdit, hLoginPwEdit);
			//GetWindowText(hLoginIdEdit, myId, sizeof(myId));
			break;
		case ID_IDLIST_OTHERID_LISTBOX:
			switch (HIWORD(wParam))
			{
				/*			case LBN_SELCHANGE:
							{
								selection = (int)SendMessage(hWnd, LB_GETCURSEL, 0, 0);
							}*/
				break;
			case LBN_DBLCLK:
			{
				int idLocal = 0;
				WCHAR recvId[10] = { 0, };
				idLocal = SendMessage(hIdListOtheridListbox, LB_GETCURSEL, 0, 0);
				SendMessage(hIdListOtheridListbox, LB_GETTEXT, idLocal, (LPARAM)recvId);
				hDlg_SendMessageProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SEND_MESSAGE), hWnd, Dlg_SendMessageProc);
				ShowWindow(hDlg_SendMessageProc, SW_SHOW);
				SendDlgItemMessage(hDlg_SendMessageProc, IDC_SEND_ID_EDIT, EM_REPLACESEL, FALSE, (LPARAM)recvId);
			}
			break;
			}
			break;
		case ID_IDLIST_WINDOW_BUTTON:
		{
			ShowWindow(hChattingRoomSecretListbox, FALSE);
			ShowWindow(hChattingRoomNameListbox, FALSE);
			ShowWindow(hChattingRoomUserNumListbox, FALSE);
			ShowWindow(hChattingRoomName, FALSE);
			ShowWindow(hChattingRoomUserNum, FALSE);
			ShowWindow(hChattingRoomCreateRoomButton, FALSE);
			ShowWindow(hChattingRoomEnterRoomButton, FALSE);
			ShowWindow(hIdListOtheridListbox, TRUE);
		}
		break;
		case ID_CHATTING_ROOM_WINDOW_BUTTON:
		{
			ShowWindow(hIdListOtheridListbox, FALSE);
			ShowWindow(hChattingRoomName, TRUE);
			ShowWindow(hChattingRoomUserNum, TRUE);
			ShowWindow(hChattingRoomSecretListbox, TRUE);
			ShowWindow(hChattingRoomNameListbox, TRUE);
			ShowWindow(hChattingRoomUserNumListbox, TRUE);
			ShowWindow(hChattingRoomCreateRoomButton, TRUE);
			ShowWindow(hChattingRoomEnterRoomButton, TRUE);
		}
		break;
		case ID_CHATTING_ROOM_NAME_LISTBOX:
			switch (HIWORD(wParam))
			{
				/*case LBN_SELCHANGE:
				{
				selection = (int)SendMessage(hWnd, LB_GETCURSEL, 0, 0);
				}*/
				break;
			case LBN_DBLCLK:
			{
				int idLocal = 0;
				WCHAR recvChattingRoomName[20] = L"";
				idLocal = SendMessage(hChattingRoomNameListbox, LB_GETCURSEL, 0, 0);
				SendMessage(hChattingRoomNameListbox, LB_GETTEXT, idLocal, (LPARAM)recvChattingRoomName);
				SendChatRoomEnter(g_serverSock, hIdListMyidEdit, hChattingRoomNameListbox);
				wcscpy(myChatRoomName, recvChattingRoomName);
			}
			break;
			}
			break;
		case ID_CHATTING_ROOM_CREATE_ROOM_BUTTON:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CREATE_CHATTING_ROOM_DIALOG), hWnd, Dlg_CreateChattingRoomProc);
			break;
		}
		break;
		case ID_CHATTING_ROOM_ENTER_ROOM_BUTTON:
		{

		}
		break;
		case ID_CHATTING_SEND_BUTTON:
			SendChatStr(g_serverSock, hDlg_SendMessageProc);
			break;
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
		HDC memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);
		BitBlt(hdc, 0, 0, 480, 480, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
	}
	break;
	////������â ũ�� �����ϴ� �ڵ�
	//case WM_GETMINMAXINFO:	//������ ũ�⳪ ��ġ �ٲٷ��� �� �� �߻��ϴ� �޽���
	//	((MINMAXINFO*)lParam)->ptMaxTrackSize.x = 500;
	//	((MINMAXINFO*)lParam)->ptMaxTrackSize.y = 470;
	//	((MINMAXINFO*)lParam)->ptMinTrackSize.x = 500;
	//	((MINMAXINFO*)lParam)->ptMinTrackSize.y = 470;
	//	return FALSE;
	case WM_DESTROY:
		PostQuitMessage(0);
		DeleteObject(hBitmap);
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

////////////////////////////////////////////////////////////////////////////////////////////
/*
ProcessSocketMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

hWnd : �޽��� �߻��� �������� �ڵ�
message : WSAAsyncSelect() �Լ� ȣ�� �� ����ߴ� �޽���
wParam : ��Ʈ��ũ �̺�Ʈ�� �߻��� ����
lParam : ���� 16��Ʈ -> �߻��� �̺�Ʈ ���� 16��Ʈ -> �����ڵ�
*/
////////////////////////////////////////////////////////////////////////////////////////////

void ProcessSocketMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SOCKET serverSock = wParam;
	SOCKADDR_IN clientAddr;
	int addrLen, retval;
	Packet header;
	if (WSAGETSELECTERROR(lParam))
	{
		//err_display(WSAGETSELECTERROR(lParam));
		//MessageBox(hWnd, (LPCWSTR)WSAGETSELECTERROR(lParam), L"WSAGETSELECTERROR(lParam)", MB_OK);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		DisplayText(L"Connect ����!");
		if (WSAAsyncSelect(serverSock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
		{
			MessageBox(hWnd, L"FD_CONNECT, WSAAsyncSelect() Error", L"WSAAsyncSelect()", MB_OK);
		}
		g_serverSock = serverSock;
		break;
	case FD_READ:
		//������ �ޱ�
		//DisplayText(L"FD_READ g_strSize ");
		retval = recv(wParam, (char*)&header, sizeof(Packet), 0);
		if (retval == SOCKET_ERROR)
		{
			MessageBox(hWnd, L"FD_READ, Packet recv() Error", L"Packet recv()", MB_OK);
			return;
		}
		switch (header.header)
		{
		case SIGNUP_RP:
		{
			bool signupSuccess;
			retval = recv(wParam, (char*)&signupSuccess, sizeof(bool), 0);
			if (signupSuccess == TRUE)
			{
				MessageBox(hWnd, L"���̵� ���� �Ϸ�", L"ȸ�� ���� ���", MB_OK);
			}
			else
			{
				MessageBox(hWnd, L"���� ����", L"ȸ�� ���� ���", MB_OK);
			}
		}
		break;
		case COMPARE_ID_RP:
		{
			bool compareidSuccess;
			retval = recv(wParam, (char*)&compareidSuccess, sizeof(bool), 0);
			if (compareidSuccess == TRUE)
			{
				MessageBox(hWnd, L"���̵� �̹� �����մϴ�.", L"���̵� Ȯ�� ���", MB_OK);
			}
			else
			{
				MessageBox(hWnd, L"���̵� ������ �� �ֽ��ϴ�.", L"���̵� Ȯ�� ���", MB_OK);

				HWND hIdEdit = GetDlgItem(hDlg_CreateIdProc, IDC_CREATE_ID_EDIT);
				HWND hIdCheckButton = GetDlgItem(hDlg_CreateIdProc, IDCHECK);
				HWND hNicknameEdit = GetDlgItem(hDlg_CreateIdProc, IDC_CREATE_NICKNAME_EDIT);
				HWND hNicknameCheckButton = GetDlgItem(hDlg_CreateIdProc, NICKNAMECHECK);
				EnableWindow(hIdEdit, FALSE);
				EnableWindow(hIdCheckButton, FALSE);
				EnableWindow(hNicknameEdit, TRUE);
				EnableWindow(hNicknameCheckButton, TRUE);
			}
		}
		break;
		case COMPARE_NICKNAME_RP:
		{
			bool compareNicknameSuccess;
			retval = recv(wParam, (char*)&compareNicknameSuccess, sizeof(bool), 0);
			if (compareNicknameSuccess == TRUE)
			{
				MessageBox(hWnd, L"���̵� �̹� �����մϴ�.", L"���̵� Ȯ�� ���", MB_OK);
			}
			else
			{
				MessageBox(hWnd, L"���̵� ������ �� �ֽ��ϴ�.", L"���̵� Ȯ�� ���", MB_OK);
				HWND hNicknameEdit = GetDlgItem(hDlg_CreateIdProc, IDC_CREATE_NICKNAME_EDIT);
				HWND hNicknameCheckButton = GetDlgItem(hDlg_CreateIdProc, NICKNAMECHECK);
				HWND hIdCheckbutton = GetDlgItem(hDlg_CreateIdProc, IDCHECK);
				HWND hIdCreatButton = GetDlgItem(hDlg_CreateIdProc, IDOK);
				EnableWindow(hNicknameEdit, FALSE);
				EnableWindow(hNicknameCheckButton, FALSE);
				EnableWindow(hIdCreatButton, TRUE);
			}
		}
		break;
		case SEARCH_PW_RP:
		{
			SearchPWData_RP searchPwData;
			retval = recv(wParam, (char*)&searchPwData, sizeof(searchPwData), 0);
			if (searchPwData.searchSuccess == TRUE)
			{
				MessageBox(hWnd, searchPwData.searchPW, L"��й�ȣ Ȯ�� ���", MB_OK);
			}
			else if (searchPwData.searchSuccess == FALSE)
			{
				MessageBox(hWnd, L"���̵�� ��������� ��ġ���� �ʽ��ϴ�", L"��й�ȣ Ȯ�� ���", MB_OK);
			}
		}
		break;
		case LOGIN_RP:
		{
			LoginData_RP loginData_RP;
			retval = recv(wParam, (char*)&loginData_RP, sizeof(LoginData_RP), 0);
			if (loginData_RP.success == TRUE)
			{
				wcscpy(myId, loginData_RP.nickname);
				CreateIdListWindow(hWnd);
			}
			else
			{
				MessageBox(hWnd, L"���̵�� ��й�ȣ�� ��ġ���� �ʽ��ϴ�", L"�α��� ���", MB_OK);
			}
		}
		break;
		case SEND_OTHER_ID_IN_NF:
		{
			OtherIdInData sendOtherIdIn;
			retval = recv(wParam, (char*)&sendOtherIdIn, sizeof(sendOtherIdIn), 0);
			SendMessage(hIdListOtheridListbox, LB_ADDSTRING, 0, (LPARAM)sendOtherIdIn.otherID);
		}
		break;
		case SEND_OTHER_ID_OUT_NF:
		{
			OtherIdOutData sendOtherIdOut;
			retval = recv(wParam, (char*)&sendOtherIdOut, sizeof(sendOtherIdOut), 0);
			WCHAR compareOtherId[10];
			int listNum = 0;
			listNum = SendMessage(hIdListOtheridListbox, LB_GETCOUNT, 0, 0);
			for (int i = 0; i < listNum; i++)
			{
				SendMessage(hIdListOtheridListbox, LB_GETTEXT, i, (LPARAM)compareOtherId);
				if (wcscmp(compareOtherId, sendOtherIdOut.otherID) == 0)
				{
					SendMessage(hIdListOtheridListbox, LB_DELETESTRING, i, 0);
					break;
				}
			}
		}
		break;
		case SINGLE_STR_DATA:
		{
			SingleData singleData;
			retval = recv(wParam, (char*)&singleData, sizeof(singleData), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, singleData recv() error", L"singleData recv()", MB_OK);
				return;
			}
			hDlg_RecvMessageProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_RECV_DIALOG), hWnd, Dlg_RecvMessageProc);
			ShowWindow(hDlg_RecvMessageProc, SW_SHOW);
			SetDlgItemText(hDlg_RecvMessageProc, IDC_RECV_ID_EDIT, singleData.id);
			SetDlgItemText(hDlg_RecvMessageProc, IDC_RECV_EDIT, singleData.str);
			break;
		}
		break;
		case SINGLE_STR_DATA_RP:
		{
			bool success = FALSE;
			retval = recv(wParam, (char*)&success, sizeof(bool), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, SINGLE_STR_DATA_RP recv() error", L"SINGLE_STR_DATA_RP recv()", MB_OK);
				return;
			}
			if (success == FALSE)
			{
				MessageBox(hWnd, L"���� ���ۿ� �����Ͽ����ϴ�.", L"���� ���� ����", MB_OK);
			}
		}
		break;
		//[ä�ù� ���� ��Ŷ ���� �� ������ ���� ���]
		case CREATE_CHATTING_ROOM_RP:
		{
			CreateChattingRoomData_RP crateChattingRoomData_RP;
			retval = recv(wParam, (char*)&crateChattingRoomData_RP, sizeof(crateChattingRoomData_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, CREATE_CHATTING_ROOM_RP recv() error", L"CREATE_CHATTING_ROOM_RP recv()", MB_OK);
				return;
			}
			//ä�ù� ���� ���� ��
			if (crateChattingRoomData_RP.success == TRUE)
			{
				//����Ʈ �ڽ���  ����, �ο���, �� ���� ����
				SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_NAME_LISTBOX, LB_ADDSTRING, 0, (LPARAM)crateChattingRoomData_RP.name);
				SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_USERNUM_LISTBOX, LB_ADDSTRING, 0, (LPARAM)crateChattingRoomData_RP.roomUserNumInfo);
				if (crateChattingRoomData_RP.pwInfo == TRUE)
				{
					SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_SECRET_LISTBOX, LB_ADDSTRING, 0, (LPARAM)L"��");
				}
				else
				{
					SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_SECRET_LISTBOX, LB_ADDSTRING, 0, (LPARAM)L" ");
				}
				//���� ���̵�, ���� ������ ä�ù� ����
				hDlg_ChatMessageProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CHAT_DIALOG), hWnd, Dlg_ChatMessageProc);
				ShowWindow(hDlg_ChatMessageProc, SW_SHOW);
				SetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_MY_ID_EDIT, myId);
				SetDlgItemText(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, myChatRoomName);	
			}
			//ä�ù� ���� ���� ��
			else
			{
				MessageBox(GetFocus(), L"ä�ù� ���� ����", L"ä�ù� ���� ���", MB_OK);
			}
		}
		break;
		case CHATTING_ROOM_ENTER_RP:
		{
			ChatEnterData_RP chatEnterData;
			retval = recv(wParam, (char*)&chatEnterData, sizeof(ChatEnterData_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, CHATTING_ROOM_ENTER_RQ recv() error", L"CHATTING_ROOM_ENTER_RQ recv()", MB_OK);
				return;
			}
			//ä�� �濡 ��й�ȣ�� ���� ���
			if (chatEnterData.success == FALSE && chatEnterData.havePW == TRUE)
			{
				//��й�ȣ ���� ���̾�α� �����ϱ�
				hDlg_CheckChatPwProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CHAT_PW_DIALOG), hWnd, Dlg_CheckChatPwProc);
				SetDlgItemText(hDlg_CheckChatPwProc, IDC_CHAT_PW_ROOM_NAME_EDIT, chatEnterData.roomName);
				ShowWindow(hDlg_CheckChatPwProc, SW_SHOW);
			}
			//ä�� �濡 ��й�ȣ�� ���� ��� �ٷ� ä�ù� ���� (������ �� ���̵�, ���� ����)
			else if (chatEnterData.success == TRUE && chatEnterData.havePW == FALSE)
			{
				hDlg_ChatMessageProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CHAT_DIALOG), hWnd, Dlg_ChatMessageProc);
				SetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_MY_ID_EDIT, myId);
				SetDlgItemText(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, chatEnterData.roomName);
				ShowWindow(hDlg_ChatMessageProc, SW_SHOW);

			}
			//ä�ù濡 ��й�ȣ�� �ְ� �������� ��� ä�ù� ���� (������ �� ���̵�, ���� ����)
			else if (chatEnterData.success == TRUE && chatEnterData.havePW == TRUE)
			{
				hDlg_ChatMessageProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CHAT_DIALOG), hWnd, Dlg_ChatMessageProc);
				SetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_MY_ID_EDIT, myId);
				SetDlgItemText(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, chatEnterData.roomName);
				ShowWindow(hDlg_ChatMessageProc, SW_SHOW);
			}
		}
		break;
		case CHATTING_ROOM_LIMIT_NUM_NF:
		{
			ChattingRoomLimitNumData chattingRoomLimitNumData;
			retval = recv(wParam, (char*)&chattingRoomLimitNumData, sizeof(ChattingRoomLimitNumData), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, CHATTING_ROOM_LIMIT_NUM_NF recv() error", L"CHATTING_ROOM_LIMIT_NUM_NF recv()", MB_OK);
				return;
			}
			SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_USERNUM_LISTBOX, LB_INSERTSTRING, chattingRoomLimitNumData.chatNumLocation, (LPARAM)chattingRoomLimitNumData.roomUserNum);
			SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_USERNUM_LISTBOX, LB_DELETESTRING, chattingRoomLimitNumData.chatNumLocation + 1, 0);
		}
		break;
		case CHECK_CHAT_PW_RP:
		{
			bool success;
			retval = recv(wParam, (char*)&success, sizeof(bool), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, CHECK_CHAT_PW_RP recv() error", L"CHECK_CHAT_PW_RP recv()", MB_OK);
				return;
			}
			if (success == FALSE)
			{
				MessageBox(GetFocus(), L"��й�ȣ �ٸ�", L"��й� ���� ���", MB_OK);
			}
			else if (success == TRUE)
			{
				DestroyWindow(hDlg_CheckChatPwProc);
				hDlg_CheckChatPwProc = NULL;
				
				//EndDialog(hDlg_CheckChatPwProc, 0);
			}
		}
		break;
		case CHATTING_ROOM_NAME_NF:
		{
			//������ �� �ο��� ���� �ޱ�
			ChattingRoomNameData chattingRoomName;
			retval = recv(wParam, (char*)&chattingRoomName, sizeof(chattingRoomName), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, chattingRoomName_NF recv() error", L"chattingRoomName_NF recv()", MB_OK);
				return;
			}
			//����Ʈ �ڽ���  ���� �ο� �� ������ ������
			SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_NAME_LISTBOX, LB_ADDSTRING, 0, (LPARAM)chattingRoomName.name);
			SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_USERNUM_LISTBOX, LB_ADDSTRING, 0, (LPARAM)chattingRoomName.roomUserNumInfo);
			if (chattingRoomName.pwInfo == TRUE)
			{
				SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_SECRET_LISTBOX, LB_ADDSTRING, 0, (LPARAM)L"��");
			}
			else
			{
				SendDlgItemMessage(hWnd, ID_CHATTING_ROOM_SECRET_LISTBOX, LB_ADDSTRING, 0, (LPARAM)L" ");
			}
		}
		break;
		case CHATTING_ROON_OUT_RP:
		{
			bool success = FALSE;
			retval = recv(wParam, (char*)&success, sizeof(bool), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, CHATTING_ROON_OUT_RP recv() error", L"CHATTING_ROON_OUT_RP recv()", MB_OK);
				return;
			}
			if (success == FALSE)
			{
				MessageBox(hWnd, L"FD_READ, ä�÷� ������ ����", L"ä�÷� ������ ����", MB_OK);
			}
		}break;
		case CHAT_OTHER_ID_IN_NF:
		{
			OtherIdInData chatOtherIdIn;
			retval = recv(wParam, (char*)&chatOtherIdIn, sizeof(chatOtherIdIn), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, chatOtherIdIn recv() error", L"chatOtherIdIn recv()", MB_OK);
				return;
			}
			SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHAT_LIST, LB_ADDSTRING, 0, (LPARAM)chatOtherIdIn.otherID);
		}
		break;
		case CHAT_OTHER_ID_OUT_NF:
		{
			OtherIdOutData chatOtherIdOut;
			retval = recv(wParam, (char*)&chatOtherIdOut, sizeof(chatOtherIdOut), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, chatOtherIdOut recv() error", L"chatOtherIdOut recv()", MB_OK);
				return;
			}
		
			int temp = 0;
			temp = SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHAT_LIST, LB_FINDSTRINGEXACT, -1, (LPARAM)chatOtherIdOut.otherID);
			SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHAT_LIST, LB_DELETESTRING, temp, 0);
			
		//	SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHAT_LIST, LB_DELETESTRING, temp, 0);
/*
			WCHAR compareOtherId[10];
			int chatUserListNum = 0;
			chatUserListNum = SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHAT_LIST, LB_GETCOUNT, 0, 0);
			for (int i = 0; i < chatUserListNum; i++)
			{
				SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHAT_LIST, LB_GETTEXT, i, (LPARAM)compareOtherId);
				WCHAR text[20] = L"";
				wsprintf(text, L"%s, %d", compareOtherId, chatUserListNum);
				MessageBox(GetFocus(), text, L"ȸ�� ���� ���", MB_OK);
				if (wcscmp(compareOtherId, chatOtherIdOut.otherID) == 0)
				{
					SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHAT_LIST, LB_DELETESTRING, i, 0);
					break;
				}
			}
		*/
		}
		break;
		case CHATTING_ROOM_DELETE_NF:
		{
			ChatRoomDeleteData chatRoomDeleteData;
			retval = recv(wParam, (char*)&chatRoomDeleteData, sizeof(chatRoomDeleteData), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, chatRoomDeleteData recv() error", L"chatRoomDeleteData recv()", MB_OK);
				return;
			}
			WCHAR compareRoomName[20];
			int chatRoomListNum = 0;
			chatRoomListNum = SendMessage(hChattingRoomNameListbox, LB_GETCOUNT, 0, 0);
			for (int i = 0; i < chatRoomListNum; i++)
			{
				SendMessage(hChattingRoomNameListbox, LB_GETTEXT, i, (LPARAM)compareRoomName);
				if (wcscmp(compareRoomName, chatRoomDeleteData.roomName) == 0)
				{
					SendMessage(hChattingRoomSecretListbox, LB_DELETESTRING, i, 0);
					SendMessage(hChattingRoomNameListbox, LB_DELETESTRING, i, 0);
					SendMessage(hChattingRoomUserNumListbox, LB_DELETESTRING, i, 0);
					break;
				}
			}
		}
		break;
		case CHAT_STR_DATA_RP:
		{
			StrData_RP strData;
			retval = recv(wParam, (char*)&strData, sizeof(StrData_RP), 0);
			if (retval == SOCKET_ERROR)
			{
				MessageBox(hWnd, L"FD_READ, StrData recv() error", L"StrData recv()", MB_OK);
				return;
			}
			SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHAT_RECV_LISTBOX, LB_ADDSTRING, 0, (LPARAM)strData.str);
		}
		break;
		}

	case FD_WRITE:
		//DisplayText(L"FD_WRITE �߻�");
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void CreateLoginWindow(HWND hWnd, LPARAM lParam)
{
	hBitmap = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	hLoginSignUpButton = CreateWindowW(L"button", L"ȸ�� ����", WS_CHILD | WS_VISIBLE | WS_BORDER,
		140, 100, 80, 22, hWnd, (HMENU)ID_LOGIN_SIGNUP_BUTTON, hInst, NULL);
	hLoginSearchPwButton = CreateWindowW(L"button", L"PW ã��", WS_CHILD | WS_VISIBLE | WS_BORDER,
		180, 410, 100, 22, hWnd, (HMENU)ID_LOGIN_SEARCH_PW_BUTTON, hInst, NULL);
	hLoginIdEdit = CreateWindowW(L"edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		180, 360, 100, 22, hWnd, (HMENU)ID_LOGIN_ID_EDIT, hInst, NULL);
	hLoginPwEdit = CreateWindowW(L"edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		180, 385, 100, 22, hWnd, (HMENU)ID_LOGIN_ID_EDIT, hInst, NULL);
	hLoginButton = CreateWindowW(L"button", L"�α���", WS_CHILD | WS_VISIBLE | WS_BORDER,
		290, 360, 50, 52, hWnd, (HMENU)ID_LOGIN_BUTTON, hInst, NULL);
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////

void CreateIdListWindow(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, TRUE);

	MoveWindow(hWnd, 800, 300, 300, 500, FALSE);
	hIdListMyidEdit = CreateWindowW(L"edit", NULL, WS_CHILD | ES_READONLY | WS_VISIBLE | WS_BORDER | WS_DISABLED,
		10, 10, 265, 22, hWnd, (HMENU)ID_IDLIST_MYID_EDIT, hInst, NULL);
	SetDlgItemText(hWnd, ID_IDLIST_MYID_EDIT, myId);

	hIdListWindowButton = CreateWindowW(L"button", L"ģ�� ���", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 40, 135, 50, hWnd, (HMENU)ID_IDLIST_WINDOW_BUTTON, hInst, NULL);
	hChattingRoomWindowButton = CreateWindowW(L"button", L"��ȭ��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		140, 40, 135, 50, hWnd, (HMENU)ID_CHATTING_ROOM_WINDOW_BUTTON, hInst, NULL);

	hIdListOtheridListbox = CreateWindowW(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		LBS_NOTIFY, 10, 95, 265, 350, hWnd, (HMENU)ID_IDLIST_OTHERID_LISTBOX, hInst, NULL);

	hChattingRoomName = CreateWindowW(L"static", L"�� �� �̸�", WS_CHILD,
		10, 95, 75, 20, hWnd, (HMENU)ID_CHATTING_ROOM_NAME_STATIC, hInst, NULL);
	hChattingRoomUserNum = CreateWindowW(L"static", L"�ο�", WS_CHILD,
		230, 95, 32, 15, hWnd, (HMENU)ID_CHATTING_ROOM_USERNUM_STATIC, hInst, NULL);
	//
	/*hChattingRoom1 = CreateWindowW(L"button", L"1�� ��", WS_CHILD | BS_PUSHBUTTON,
		10, 100, 130, 130, hWnd, (HMENU)ID_CHATTING_ROOM_BUTTON1, hInst, NULL);
	hChattingRoom2 = CreateWindowW(L"button", L"2�� ��", WS_CHILD | BS_PUSHBUTTON,
		145, 100, 130, 130, hWnd, (HMENU)ID_CHATTING_ROOM_BUTTON2, hInst, NULL);
	hChattingRoom3 = CreateWindowW(L"button", L"3�� ��", WS_CHILD | BS_PUSHBUTTON,
		10, 240, 130, 130, hWnd, (HMENU)ID_CHATTING_ROOM_BUTTON3, hInst, NULL);
	hChattingRoom4 = CreateWindowW(L"button", L"4�� ��", WS_CHILD | BS_PUSHBUTTON,
		145, 240, 130, 130, hWnd, (HMENU)ID_CHATTING_ROOM_BUTTON4, hInst, NULL);*/

	hChattingRoomSecretListbox = CreateWindowW(L"listbox", NULL, WS_CHILD | WS_BORDER | LBS_NOTIFY | WS_DISABLED,
		10, 110, 20, 300, hWnd, (HMENU)ID_CHATTING_ROOM_SECRET_LISTBOX, hInst, NULL);

	hChattingRoomNameListbox = CreateWindowW(L"listbox", NULL, WS_CHILD | WS_BORDER |
		LBS_NOTIFY, 30, 110, 210, 300, hWnd, (HMENU)ID_CHATTING_ROOM_NAME_LISTBOX, hInst, NULL);

	hChattingRoomUserNumListbox = CreateWindowW(L"listbox", NULL, WS_CHILD | WS_BORDER | LBS_NOTIFY | WS_DISABLED,
		240, 110, 35, 300, hWnd, (HMENU)ID_CHATTING_ROOM_USERNUM_LISTBOX, hInst, NULL);

	hChattingRoomCreateRoomButton = CreateWindowW(L"button", L" �� ����� ", WS_CHILD | BS_PUSHBUTTON,
		10, 400, 135, 30, hWnd, (HMENU)ID_CHATTING_ROOM_CREATE_ROOM_BUTTON, hInst, NULL);
	hChattingRoomEnterRoomButton = CreateWindowW(L"button", L" ���� �ϱ� ", WS_CHILD | BS_PUSHBUTTON,
		140, 400, 135, 30, hWnd, (HMENU)ID_CHATTING_ROOM_ENTER_ROOM_BUTTON, hInst, NULL);



	DestroyWindow(hLoginIdEdit);
	DestroyWindow(hLoginPwEdit);
	DestroyWindow(hLoginSignUpButton);
	DestroyWindow(hLoginSearchPwButton);
	DestroyWindow(hLoginButton);
	//for (int i = 0; i<5; i++)
	//	SendMessage(hIdListOtheridListbox, LB_ADDSTRING, 0, (LPARAM)Items[i]);
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void CreateChattingWindow(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, TRUE);

	MoveWindow(hWnd, 200, 200, 500, 500, FALSE);
	hChatWriteEdit = CreateWindowW(L"edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		//	WS_VSCROLL		|									//���� ��ũ�� ����
		//	ES_AUTOHSCROLL	| ES_AUTOVSCROLL | ES_MULTILINE		//Enter Ű ����
		5, 385, 425, 50, hWnd, (HMENU)ID_CHATTING_WRITE_EDIT, hInst, NULL);
	hChatReadListbox = CreateWindowW(L"listbox", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_VSCROLL |
		ES_READONLY,
		5, 5, 475, 380, hWnd, (HMENU)ID_CHATTING_READ_LISTBOX, hInst, NULL);
	hChatSendButton = CreateWindowW(L"button", L"������", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		430, 385, 50, 50, hWnd, (HMENU)ID_CHATTING_SEND_BUTTON, hInst, NULL);
	DestroyWindow(hLoginIdEdit);
	DestroyWindow(hLoginPwEdit);
	DestroyWindow(hLoginSignUpButton);
	DestroyWindow(hLoginSearchPwButton);
	DestroyWindow(hLoginButton);
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void InitClientServer(HWND hWnd)
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;
	//socket ����
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == INVALID_SOCKET)
		MessageBox(hWnd, L"CreateSock, socket error", L"clientSock == INVALID_SOCKET", MB_OK);
	//WSAAsyncSelect()�� connect ���
	if (WSAAsyncSelect(clientSock, hWnd, WM_SOCKET, FD_CONNECT | FD_CLOSE) == SOCKET_ERROR)
		MessageBox(hWnd, L"CreateSock, WSAAsyncSelect() error", L"WSAAsyncSelect()", MB_OK);
	int retval;
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(clientSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	DisplayText(L"���� ������...!");
}


////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void DisplayText(LPWSTR fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	WCHAR cbuf[BUFSIZE + 256];
	vswprintf(cbuf, BUFSIZE + 256, fmt, arg);

	/*int nLength = GetWindowTextLength(hChatWriteEdit);
	SendMessage(hChatWriteEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hChatWriteEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);*/
	SendMessage(hChatReadListbox, LB_ADDSTRING, 0, (LPARAM)cbuf);

	va_end(arg);
}

//void err_quit(LPWSTR msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf, 0, NULL);
//	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
//	LocalFree(lpMsgBuf);
//	exit(1);
//}
//
//void err_display(LPWSTR msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf, 0, NULL);
//	DisplayText(L"[%s] %s", msg, (char *)lpMsgBuf);
//	LocalFree(lpMsgBuf);
//}
//
//void err_display(int errcode)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, errcode,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf, 0, NULL);
//	DisplayText(L"[����] %s", (char *)lpMsgBuf);
//	LocalFree(lpMsgBuf);
//}