#pragma once
#include<winsock2.h>
#include "Protocal.h"
#include "Resource.h"

void SendSignUp_RQ(SOCKET sock, HWND hDlg);
void SendSignUp_RQ(SOCKET sock, HWND hDlg);
void SendIdCheck_RQ(SOCKET sock, HWND hDlg);
void SendNicknameCheck_RQ(SOCKET sock, HWND hDlg);
void SendSearchPW_RQ(SOCKET sock, HWND hDlg);
void SendLogin_RQ(SOCKET sock, HWND hLoginIdEdit, HWND hLoginPwEdit);
void SendSingleStr(SOCKET sock, HWND hDlg_SendMessageProc, LPWSTR recvId);
//void SendSingleStr(SOCKET sock, HWND hDlg_SendMessageProc, HWND hIdListOtheridListbox);
void CreateChattingRoom(SOCKET sock,int num, HWND hDlg_CreateChattingRoomProc);
void SendChatRoomEnter(SOCKET sock, HWND hIdListMyidEdit, HWND hChattingRoomNameListbox);
//void SendChatRoomEnter(SOCKET sock, HWND hIdListMyidEdit);
void SendChatRoomOut(SOCKET, HWND hDlg_ChatMessageProc);
//void SendChatRoomOut(SOCKET sock);
void SendChatStr(SOCKET sock, HWND hDlg_ChatMessageProc);
void CheckChatPW_RQ(SOCKET sock, HWND hDlg_CheckChatPwProc, LPWSTR myId);