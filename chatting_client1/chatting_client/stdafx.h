// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#pragma warning(disable: 4996)
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>
#include<winsock2.h>
// C ��Ÿ�� ��� �����Դϴ�.
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.



//��� ���� 

//���� �޼��� ���
//SendDlgItemMessage(hDlg, IDC_CHAT_RECV_EDIT, EM_REPLACESEL, FALSE, (LPARAM)L"ok��ư");
//������ ��� �߰�
//SendDlgItemMessage(hDlg, IDC_CHAT_LIST, LB_ADDSTRING, 0, (LPARAM)L"ok��ư");
//�� ���̵� ���
//SendDlgItemMessage(hDlg, IDC_CHAT_MY_ID_EDIT, EM_REPLACESEL, FALSE, (LPARAM)myId);

//����Ʈ Ŭ���� �� ��ǥ�� �� ���� �� ��������
//int idLocal = 0;
//WCHAR recvId[10] = { 0, };
//idLocal = SendMessage(hIdListOtheridListbox, LB_GETCURSEL, 0, 0);
//SendMessage(hIdListOtheridListbox, LB_GETTEXT, idLocal, (LPARAM)recvId);

//�𵨸��� ���̾�α� ����� â ����
//hDlg_SendMessageProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SEND_MESSAGE), hWnd, Dlg_SendMessageProc);
//ShowWindow(hDlg_SendMessageProc, SW_SHOW);

//�޽��� �ڽ��� ���� ����ϴ� ���
/*WCHAR text[20];
wsprintf(text, L"���� ũ�� : %s", roomName);
MessageBox(GetFocus(), text, L"ȸ�� ���� ���", MB_OK);*/

//Set �� Send �� ����
//Set�� ���� �Է��� ��
//Send�� ����Ʈ �ڽ� ���� �Ϳ� ����� �� // �޼����� ����
//SetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_MY_ID_EDIT, myId);
//SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, EM_REPLACESEL, FALSE, (LPARAM)myChatRoomName);