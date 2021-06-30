#include "ClientManager.h"
#include "Protocal.h"

ClientInfoNode::ClientInfoNode()
{
	memset(id, 0, sizeof(id));
	size = 0;
	next = nullptr;
}

ClientManager::ClientManager()
{
	memset(roomName, 0, sizeof(roomName));
	memset(roomPW, 0, sizeof(roomPW));
	roomUserNum = 0;
	roomSize = 0;
	next = nullptr;
	head = nullptr;
	tail = nullptr;
}

ClientInfoNode* ClientManager::GetClientInfo(SOCKET sock)
{
	ClientInfoNode *ptr = head;

	while (ptr)
	{
		if (ptr->sock == sock)
			return ptr;
		ptr = ptr->next;
	}
	return nullptr;
}

void ClientManager::AddClientInfo(SOCKET sock, LPWSTR id)
{
	ClientInfoNode *node = new ClientInfoNode;
	node->size = node->size + 1;
	node->sock = sock;
	wcscpy(node->id, id);
	node->next = nullptr;

	if (head == nullptr)
		head = node;
	else
		tail->next = node;
	tail = node;
}



void ClientManager::RemoveClientInfo(SOCKET sock, int protocal)
{
	OtherId_Out_NF sendOtherIdOut_NF;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	int retval;
	getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);
	//DisplayText(L"[TCP 서버] 클라 종료 : IP 주소 %S, 포트번호 = %d \r\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	ClientInfoNode *curr = head;
	ClientInfoNode *prev = nullptr;
	sendOtherIdOut_NF.otherIdHeader.header = protocal;
	while (curr != nullptr)
	{
		if (curr->sock == sock)
		{
			if (prev != nullptr)
			{
				if (curr == tail)
					tail = prev;
				prev->next = curr->next;
			}
			else
			{
				head = curr->next;
				tail = curr->next;
			}
			wcscpy(sendOtherIdOut_NF.otherIdOutData.otherID, curr->id);
			if(protocal == SEND_OTHER_ID_OUT_NF)
				closesocket(curr->sock);
			delete curr;
			ClientInfoNode *ptr = head;
			while (ptr)
			{
				retval = send(ptr->sock, (char*)&sendOtherIdOut_NF, sizeof(sendOtherIdOut_NF), 0);
				if (retval == SOCKET_ERROR)
				{
					//err_display(L"sendOtherIdOut_NF send()");
					return;
				}
				ptr = ptr->next;
			}
			return;
		}

		curr = curr->next;
	}

}

bool ClientManager::CheckUserNum(int roomUserNum)
{
	if (roomSize < this->roomUserNum + roomUserNum)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

LPWSTR ClientManager::FindId(SOCKET sock)
{
	ClientInfoNode *ptr = head;
	WCHAR findId[10] = L"";
	while (ptr)
	{
		if (ptr->sock == sock)
		{
			return ptr->id;
		}
		ptr = ptr->next;
	}
	return nullptr;
}

ChattingRoomManager::ChattingRoomManager()
{
	head = nullptr;
	tail = nullptr;
}

void ChattingRoomManager::AddClientManager(LPWSTR roomName, LPWSTR roomPW, int roomSize, SOCKET sock, LPWSTR id)
{
	ClientManager *node = new ClientManager;
	node->roomUserNum = node->roomUserNum + 1;
	wcscpy(node->roomName, roomName);
	wcscpy(node->roomPW, roomPW);
	node->roomSize = roomSize;
	node->next = nullptr;
	node->AddClientInfo(sock, id);
	if (head == nullptr)
		head = node;
	else
		tail->next = node;
	tail = node;
	
}

void ChattingRoomManager::RemoveClientManager(ClientManager* removeChattingRoom, ClientManager* preNode)
{
	if (head == removeChattingRoom)
	{
		head = head->next;
		if (tail == removeChattingRoom)
			tail = head;
	}
	else if (tail == removeChattingRoom)
		tail = preNode;
	else
		preNode->next = removeChattingRoom->next;
	delete removeChattingRoom;
}
