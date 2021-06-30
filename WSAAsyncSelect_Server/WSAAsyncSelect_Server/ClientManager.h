#pragma once
#include<winsock2.h>

class ClientInfoNode
{
	//friend class ClientManager;
public:
	SOCKET sock;
	WCHAR id[10];
	WCHAR nickname[10];
	int UniqueNumber;
	int size;
	ClientInfoNode* next;
	ClientInfoNode();
};

class ClientManager
{
	//friend class ChattingRoomManager;
public:
	WCHAR roomName[20];
	WCHAR roomPW[10];
	int   roomUserNum;
	int	  roomSize;
	ClientManager* next;
	ClientInfoNode* head;
	ClientInfoNode* tail;

	ClientManager();
	ClientInfoNode* GetClientInfo(SOCKET sock);
	void AddClientInfo(SOCKET sock, LPWSTR id);
	void RemoveClientInfo(SOCKET sock, int protocal);
	bool CheckUserNum(int roomUserNum);
	LPWSTR FindId(SOCKET sock);
};

class ChattingRoomManager
{
public:
	ClientManager* head;
	ClientManager* tail;

	ChattingRoomManager();
	void AddClientManager(LPWSTR roomName, LPWSTR roomPW, 
		                  int roomSize, SOCKET sock, LPWSTR id);
	void RemoveClientManager(ClientManager* removeChattingRoom, ClientManager* preNode);

};