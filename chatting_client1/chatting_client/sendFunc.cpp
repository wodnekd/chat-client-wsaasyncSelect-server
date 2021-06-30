#include "sendFunc.h"
#include<stdio.h>
#include <windows.h>

void SendSignUp_RQ(SOCKET sock, HWND hDlg)
{
	int retval;
	SignUp_RQ signup;
	GetDlgItemText(hDlg, IDC_CREATE_ID_EDIT, signup.signupData.createID, sizeof(signup.signupData.createID));
	GetDlgItemText(hDlg, IDC_CREATE_PW_EDIT, signup.signupData.createPW, sizeof(signup.signupData.createPW));
	GetDlgItemText(hDlg, IDC_CREATE_NICKNAME_EDIT, signup.signupData.createNickname, sizeof(signup.signupData.createNickname));
	GetDlgItemText(hDlg, IDC_CREATE_QUSET_EDIT, signup.signupData.createQuest, sizeof(signup.signupData.createQuest));
	retval = send(sock, (char*)&signup, sizeof(SignUp_RQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendSignUp_RQ()");
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void SendIdCheck_RQ(SOCKET sock, HWND hDlg)
{
	int retval;
	CompareID_RQ compareid;
	GetDlgItemText(hDlg, IDC_CREATE_ID_EDIT, compareid.compareIdData.compareChar, sizeof(compareid.compareIdData.compareChar));
	retval = send(sock, (char*)&compareid, sizeof(compareid), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void SendNicknameCheck_RQ(SOCKET sock, HWND hDlg)
{
	int retval;
	CompareNickname_RQ compareNickname;
	GetDlgItemText(hDlg, IDC_CREATE_NICKNAME_EDIT, compareNickname.data.compareChar, sizeof(compareNickname.data.compareChar));
	retval = send(sock, (char*)&compareNickname, sizeof(compareNickname), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void SendSearchPW_RQ(SOCKET sock, HWND hDlg)
{
	int retval;
	SearchPW_RQ searchPw;
	GetDlgItemText(hDlg, IDC_SEARCH_PW_ID_EDIT, searchPw.searchPwData.loginID, sizeof(searchPw.searchPwData.loginID));
	GetDlgItemText(hDlg, IDC_SEARCH_PW_QUEST_EDIT, searchPw.searchPwData.loginQuest, sizeof(searchPw.searchPwData.loginQuest));
	retval = send(sock, (char*)&searchPw, sizeof(SearchPW_RQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendSearchPW_RQ()");
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void SendLogin_RQ(SOCKET sock, HWND hLoginIdEdit, HWND hLoginPwEdit)
{
	int retval;
	Login_RQ login;
	GetWindowText(hLoginIdEdit, login.loginData.loginID, sizeof(login.loginData.loginID));
	GetWindowText(hLoginPwEdit, login.loginData.loginPW, sizeof(login.loginData.loginPW));
	retval = send(sock, (char*)&login, sizeof(Login_RQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendLogin_RQ()");
		return;
	}
}

void SendSingleStr(SOCKET sock, HWND hDlg_SendMessageProc, LPWSTR recvId)
{
	int retval;
	SingleStrData single;
	GetDlgItemText(hDlg_SendMessageProc, IDC_MESSAGE_EDIT, single.SingleData.str, sizeof(single.SingleData.str));
	wcscpy(single.SingleData.id, recvId);
	retval = send(sock, (char*)&single, sizeof(SingleStrData), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"send()");
		return;
	}

}
//채팅방 생성 요청 패킷 보내는 함수
void CreateChattingRoom(SOCKET sock, int num, HWND hDlg_CreateChattingRoomProc)
{
	int retval;
	//다이얼로그에 입력한 방제, 비밀번호, 인원 수 정보 패킷에 저장
	CreateChattingRoom_RQ createChattingRoom;
	GetDlgItemText(hDlg_CreateChattingRoomProc, IDC_CREATE_CHATTING_ROOM_NAME_EDIT, createChattingRoom.data.name, sizeof(createChattingRoom.data.name));
	GetDlgItemText(hDlg_CreateChattingRoomProc, IDC_CREATE_CHATTING_ROOM_PW_EDIT, createChattingRoom.data.pw, sizeof(createChattingRoom.data.pw));
	createChattingRoom.data.roomSize = num;
	//패킷 전송
	retval = send(sock, (char*)&createChattingRoom, sizeof(createChattingRoom), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"CreateChattingRoom_RQ()");
		return;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////
///*
//GetDlgItemText : 보내기 다이얼로그에 있는 메시지 데이터 받아와서 single.SingleData.str에 저장 후 전송
//i = SendMessage : 현재 커서의 위치 가져온다 아이디 가져오는 것에 사용
//SendMessage : 커서가 있는 아이디 가져와서 single.SingleData.id에 저장 (보내고 싶은 아이디)
//*/
//////////////////////////////////////////////////////////////////////////////////////////////
//void SendSingleStr(SOCKET sock, HWND hDlg_SendMessageProc, HWND hIdListOtheridListbox)
//{
//	int retval;
//	int i;
//	//WCHAR text[100];
//	SingleStrData single;
//	GetDlgItemText(hDlg_SendMessageProc, IDC_MESSAGE_EDIT, single.SingleData.str, sizeof(single.SingleData.str));
//	i = SendMessage(hIdListOtheridListbox, LB_GETCURSEL, 0, 0);
//	SendMessage(hIdListOtheridListbox, LB_GETTEXT, i, (LPARAM)single.SingleData.id);
//	retval = send(sock, (char*)&single, sizeof(SingleStrData), 0);
//	if (retval == SOCKET_ERROR)
//	{
//		//err_display(L"send()");
//		return;
//	}
//
//}
////////////////////////////////////////////////////////////////////////////////////////////

/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void  SendChatRoomEnter(SOCKET sock, HWND hIdListMyidEdit, HWND hChattingRoomNameListbox)
{
	ChatEnter_RQ chatEnterRoom;
	int retval;
	int idLocal = 0;
	idLocal = SendMessage(hChattingRoomNameListbox, LB_GETCURSEL, 0, 0);
	SendMessage(hChattingRoomNameListbox, LB_GETTEXT, idLocal, (LPARAM)chatEnterRoom.chatEnterData.chattingRoomName);
	GetWindowText(hIdListMyidEdit, chatEnterRoom.chatEnterData.loginID, sizeof(chatEnterRoom.chatEnterData.loginID));
	retval = send(sock, (char*)&chatEnterRoom, sizeof(ChatEnter_RQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendLogin_RQ()");
		return;
	}
}
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
//void  SendChatRoomEnter(SOCKET sock, HWND hIdListMyidEdit)
//{
//	int retval;
//	ChatEnter_RQ chatLogin;
//	GetWindowText(hIdListMyidEdit, chatLogin.chatEnterData.loginID, sizeof(chatLogin.chatEnterData.loginID));
//	//chatLogin.chatEnterData.loginSock = sock;
//	retval = send(sock, (char*)&chatLogin, sizeof(ChatEnter_RQ), 0);
//	if (retval == SOCKET_ERROR)
//	{
//		//err_display(L"SendLogin_RQ()");
//		return;
//	}
//}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/

////////////////////////////////////////////////////////////////////////////////////////////
void  SendChatRoomOut(SOCKET sock, HWND hDlg_ChatMessageProc)
{
	int retval;
	ChatOut_RQ chatOut_RQ;
	GetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_MY_ID_EDIT, chatOut_RQ.data.id, sizeof(chatOut_RQ.data.id));
	GetDlgItemText(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, chatOut_RQ.data.roomName, sizeof(chatOut_RQ.data.roomName));
	retval = send(sock, (char*)&chatOut_RQ, sizeof(chatOut_RQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendLogin_RQ()");
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void  SendChatRoomOut(SOCKET sock)
{
	int retval;
	ChatOut_RQ chatOut;
	retval = send(sock, (char*)&chatOut, sizeof(ChatOut_RQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendLogin_RQ()");
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
/*
*/
////////////////////////////////////////////////////////////////////////////////////////////
void SendChatStr(SOCKET sock, HWND hDlg_ChatMessageProc)
{
	int retval;
	ChatStrData_RQ chat;
	//GetWindowText(hChatWriteEdit, chat.chatData.str, sizeof(chat.chatData.str));
	GetDlgItemText(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, chat.chatData.chatRoomName, sizeof(chat.chatData.chatRoomName));
	GetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_SEND_EDIT, chat.chatData.str, sizeof(chat.chatData.str));

	retval = send(sock, (char*)&chat, sizeof(ChatStrData_RQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"send()");
		return;
	}

}

void CheckChatPW_RQ(SOCKET sock, HWND hDlg_CheckChatPwProc, LPWSTR myId)
{
	int retval;
	CheckChatPw_RQ checkChatPW_RQ;
	GetDlgItemText(hDlg_CheckChatPwProc, IDC_CHAT_PW_ROOM_NAME_EDIT, checkChatPW_RQ.data.roomName, 20);
	GetDlgItemText(hDlg_CheckChatPwProc, IDC_CHAT_PW_PW_EDIT, checkChatPW_RQ.data.pw, 10);
	wcscpy(checkChatPW_RQ.data.id, myId);
	retval = send(sock, (char*)&checkChatPW_RQ, sizeof(CheckChatPw_RQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"send()");
		return;
	}

}
