#pragma once
#include <windows.h>
/*
Request == Ŭ�� -> ������ ��û
Reply	== ���� -> Ŭ�� ����
Notify	== ���� -> Ŭ�� �뺸 (�ٸ� Ŭ���� �ൿ�� ���� ��)
Command == ���� -> Ŭ�� ��� (������ ���� Ŭ�� ������ ��)

*/
enum Protocal
{
	LOGIN_RQ = 300,
	LOGIN_RP,
	SIGNUP_RQ,
	SIGNUP_RP,
	SEND_OTHER_ID_IN_NF,
	SEND_OTHER_ID_OUT_NF,
	SEARCH_PW_RQ,
	SEARCH_PW_RP,
	COMPARE_ID_RQ,
	COMPARE_ID_RP,
	COMPARE_NICKNAME_RQ,
	COMPARE_NICKNAME_RP,
	SINGLE_STR_DATA,
	SINGLE_STR_DATA_RP,
	CREATE_CHATTING_ROOM_RQ,
	CREATE_CHATTING_ROOM_RP,
	CHATTING_ROOM_NAME_NF,
	CHATTING_ROOM_ENTER_RQ,
	CHATTING_ROOM_ENTER_RP,
	CHATTING_ROOM_LIMIT_NUM_NF,
	CHECK_CHAT_PW_RQ,
	CHECK_CHAT_PW_RP,
	CHATTING_ROON_OUT_RQ,
	CHATTING_ROON_OUT_RP,
	CHATTING_ROOM_DELETE_NF,
	CHAT_OTHER_ID_IN_NF,
	CHAT_OTHER_ID_OUT_NF,
	CHAT_STR_DATA_RQ,
	CHAT_STR_DATA_RP
};
/*
����� ��� ���� �������� ����� �����ϴ� ��Ŷ
size = 8byte;
*/
struct Packet
{
	int header;
	int dataSize;
};

/////////////////////////////////////////////////////////////////////
/*
ȸ������ �ϴ� ��Ŷ
*/
//ȸ������ ������ (id, pw, nickname, quest) 
struct SignUpData
{
	WCHAR createID[10];
	WCHAR createPW[10];
	WCHAR createNickname[10];
	WCHAR createQuest[10];
	SignUpData()
	{
		wcscpy(createID, L"");
		wcscpy(createPW, L"");
		wcscpy(createNickname, L"");
		wcscpy(createQuest, L"");
	}
};
//[Ŭ�� -> ����] id, pw, nickname, ���� ��Ŷ ����
struct SignUp_RQ
{
	Packet signupHeader;
	SignUpData signupData;
	SignUp_RQ()
	{
		signupHeader.header = SIGNUP_RQ;
		signupHeader.dataSize = sizeof(SignUpData);
	}
};
//[���� -> Ŭ��] ȸ������ ���� ���� ���� ��Ŷ ����
struct SignUp_RP
{
	Packet signupHeader;
	bool signupSucess;
	SignUp_RP()
	{
		signupHeader.header = SIGNUP_RP;
		signupHeader.dataSize = sizeof(bool);
		signupSucess = FALSE;
	}
};

/////////////////////////////////////////////////////////////////////
/*
���� �� �ٸ� Ŭ���̾�Ʈ ID�� �˷��ִ� ��Ŷ
*/
//���̵� �˷��ִ� ������ (id)
struct OtherIdInData
{
	WCHAR otherID[10];
	OtherIdInData()
	{
		wcscpy(otherID, L"");
	}
};
//[���� -> Ŭ��] ���� �Ǿ� �ִ� Ŭ�� ��ο��� ��Ŷ ����
struct OtherId_In_NF
{
	Packet otherIdInHeader;
	OtherIdInData otherIdInData;
	OtherId_In_NF()
	{
		otherIdInHeader.header = SEND_OTHER_ID_IN_NF;
		otherIdInHeader.dataSize = sizeof(OtherIdInData);
	}
};

/////////////////////////////////////////////////////////////////////
/*
���� �� �ٸ� Ŭ���̾�Ʈ ID�� �˷��ִ� ��Ŷ
*/
// ���̵� �˷��ִ� ������ (id)
struct OtherIdOutData
{
	WCHAR otherID[10];
	OtherIdOutData()
	{
		wcscpy(otherID, L"");
	}
};
//[���� -> Ŭ��] ���� �Ǿ� �ִ� Ŭ�� ��忡�� ��Ŷ ����
struct OtherId_Out_NF
{
	Packet otherIdHeader;
	OtherIdOutData otherIdOutData;
	OtherId_Out_NF()
	{
		otherIdHeader.header = SEND_OTHER_ID_OUT_NF;
		otherIdHeader.dataSize = sizeof(OtherIdOutData);
	}
};
/////////////////////////////////////////////////////////////////////
/*
ȸ�� ���� �� ���̵� Ȯ�� ��Ŷ
*/
// ���̵� Ȯ�� ������ (id)
struct CompareData
{
	WCHAR compareChar[10];
	CompareData()
	{
		wcscpy(compareChar, L"");
	}
};
//[Ŭ�� -> ����] ���̵� �ߺ� Ȯ�� ��Ŷ ����
struct CompareID_RQ
{
	Packet compareIdHeader;
	CompareData compareIdData;
	CompareID_RQ()
	{
		compareIdHeader.header = COMPARE_ID_RQ;
		compareIdHeader.dataSize = sizeof(CompareData);
	}
};
//[���� -> Ŭ��] ���̵� �ߺ� Ȯ�� ��� ��Ŷ ����
struct CompareID_RP
{
	Packet compareIdHeader;
	bool compareIdSueccess;
	CompareID_RP()
	{
		compareIdHeader.header = COMPARE_ID_RP;
		compareIdHeader.dataSize = sizeof(bool);
		compareIdSueccess = FALSE;
	}
};

//[Ŭ�� -> ����] �г��� �ߺ� Ȯ�� ��Ŷ ����
struct CompareNickname_RQ
{
	Packet compareNicknameHeader;
	CompareData data;
	CompareNickname_RQ()
	{
		compareNicknameHeader.header = COMPARE_NICKNAME_RQ;
		compareNicknameHeader.dataSize = sizeof(CompareData);
	}
};
//[���� -> Ŭ��] ���̵� �ߺ� Ȯ�� ��� ��Ŷ ����
struct CompareNickname_RP
{
	Packet compareNicknameHeader;
	bool compareNicknameSueccess;
	CompareNickname_RP()
	{
		compareNicknameHeader.header = COMPARE_NICKNAME_RP;
		compareNicknameHeader.dataSize = sizeof(bool);
		compareNicknameSueccess = FALSE;
	}
};

/////////////////////////////////////////////////////////////////////
/*
��й�ȣ ã�� ��Ŷ
*/
//��й�ȣ ã�� ������ (id, quest)
struct SearchPWData_RQ
{
	WCHAR loginID[10];
	WCHAR loginQuest[10];
	SearchPWData_RQ()
	{
		wcscpy(loginID, L"");
		wcscpy(loginQuest, L"");
	}
};
//[Ŭ�� -> ����] ��й�ȣ ã�� ���� ������ ��Ŷ
struct SearchPW_RQ
{
	Packet searchPwHeader;
	SearchPWData_RQ searchPwData;
	SearchPW_RQ()
	{
		searchPwHeader.header = SEARCH_PW_RQ;
		searchPwHeader.dataSize = sizeof(SearchPWData_RQ);
	}
};
//��й�ȣ ������ (pw, success)
struct SearchPWData_RP
{
	WCHAR searchPW[10];
	bool searchSuccess;
	SearchPWData_RP()
	{
		wcscpy(searchPW, L"");
		searchSuccess = FALSE;
	}
};
//[���� -> Ŭ��] ��й�ȣ ã�� ���� ���� Ȯ�� �� ������ ��Ŷ 
struct SearchPW_RP
{
	Packet searchPwHeader;
	SearchPWData_RP searchPwData;
	SearchPW_RP()
	{
		searchPwHeader.header = SEARCH_PW_RP;
		searchPwHeader.dataSize = sizeof(SearchPWData_RP);
	}
};
/////////////////////////////////////////////////////////////////////
/*
�α��� �� ���̵� ��й�ȣ Ȯ���ϴ� ��Ŷ
*/
//�α��� ������ (id, pw)
struct LoginData
{
	WCHAR loginID[10];
	WCHAR loginPW[10];
	LoginData()
	{
		wcscpy(loginID, L"");
		wcscpy(loginPW, L"");
	}
};
//[Ŭ�� -> ����] �α��� Ȯ���ϱ� ���� ������ ��Ŷ 
struct Login_RQ
{
	Packet loginHeader;
	LoginData loginData;
	Login_RQ()
	{
		loginHeader.header = LOGIN_RQ;
		loginHeader.dataSize = sizeof(LoginData);
	}
};

//�α��� Ȯ�� �� ���� ���� ������ (nickname, success)
struct LoginData_RP
{
	WCHAR nickname[10];
	bool success;
	LoginData_RP()
	{
		wcscpy(nickname, L"");
		success = FALSE;
	}
};
//[���� -> Ŭ��] �α��� Ȯ�� �� ���� ���� ������ ��Ŷ
struct Login_RP
{
	Packet loginHeader;
	LoginData_RP loginData_RP;
	Login_RP()
	{
		loginHeader.header = LOGIN_RP;
		loginHeader.dataSize = sizeof(bool);
	}
};
/////////////////////////////////////////////////////////////////////
/*
���� ������ 1:1 ���ڿ� ������ ��Ŷ
*/
// ���� ������ ������ (id, str)
struct SingleData
{
	WCHAR  id[10];
	WCHAR str[50];
	SingleData()
	{
		wcscpy(id, L"");
		wcscpy(str, L"");
	}
};
//[Ŭ�� -> ����] 1:1 ���ڿ� ���� ��Ŷ
struct SingleStrData
{
	Packet singleStrHeader;
	SingleData SingleData;
	SingleStrData()
	{
		singleStrHeader.header = SINGLE_STR_DATA;
		singleStrHeader.dataSize = sizeof(SingleData);
	}
};
//[���� -> Ŭ��] ���ڿ� ���� ���� ���� Ȯ�� ��Ŷ
struct SingleStrData_RP

{
	Packet singleStrHeader;
	bool success;
	SingleStrData_RP()
	{
		singleStrHeader.header = SINGLE_STR_DATA_RP;
		singleStrHeader.dataSize = sizeof(bool);
		success = FALSE;
	}

};
/////////////////////////////////////////////////////////////////////
/*
ä�ù� ���� ��û ������ ��Ŷ
*/
//ä�ù� ������ �ʿ��� ���� ������ (name, pw, roomSize)
struct CreateChattingRoomData
{
	WCHAR name[20];
	WCHAR pw[10];
	int	roomSize;
	CreateChattingRoomData()
	{
		wcscpy(name, L"");
		wcscpy(pw, L"");
		roomSize = 0;
	}
};
//[Ŭ�� -> ����] ä�ù� ���� ��û ��Ŷ ����
struct CreateChattingRoom_RQ
{
	Packet createChattingRoomHeader;
	CreateChattingRoomData data;
	CreateChattingRoom_RQ()
	{
		createChattingRoomHeader.header = CREATE_CHATTING_ROOM_RQ;
		createChattingRoomHeader.dataSize = sizeof(CreateChattingRoomData);
	}
};
//ä�ù� ���� �Ϸ�� ���۵Ǵ� ������ (name, roomUserNumInfo, success)
struct CreateChattingRoomData_RP
{
	WCHAR name[20];
	WCHAR roomUserNumInfo[4];
	bool pwInfo;
	bool success;
	CreateChattingRoomData_RP()
	{
		wcscpy(name, L"");
		wcscpy(roomUserNumInfo, L"");
		pwInfo = FALSE;
		success = FALSE;
	}
};
//[���� -> Ŭ��] ä�ù� ���� ���� ���� ��Ŷ ����
struct CreateChattingRoom_RP
{
	Packet createChattingRoomHeader;
	CreateChattingRoomData_RP data;
	CreateChattingRoom_RP()
	{
		createChattingRoomHeader.header = CREATE_CHATTING_ROOM_RP;
		createChattingRoomHeader.dataSize = sizeof(CreateChattingRoomData_RP);
	}
};

/////////////////////////////////////////////////////////////////////
/*
���� â ä�� ����Ʈ�ڽ��� �ο��� �ѷ��ִ� ��Ŷ
*/

//ä�ù� ����� 
struct ChattingRoomLimitNumData
{
	WCHAR roomUserNum[4];
	int chatNumLocation;
	ChattingRoomLimitNumData()
	{
		wcscpy(roomUserNum, L"");
		chatNumLocation = 0;
	}
};


//
struct ChattingRoomLimitNum_NF
{
	Packet chattingRoomLimitNumHeader;
	ChattingRoomLimitNumData data;
	ChattingRoomLimitNum_NF()
	{
		chattingRoomLimitNumHeader.header = CHATTING_ROOM_LIMIT_NUM_NF;
		chattingRoomLimitNumHeader.dataSize = sizeof(ChattingRoomLimitNumData);
	}
};

/////////////////////////////////////////////////////////////////////
/*
���� â ä�� ����Ʈ�ڽ��� ���� �ѷ��ִ� ��Ŷ
*/
//ä�ù� ��� ����Ʈ �ڽ��� �� ������ (name, roomUserNumInfo, pwInfo)
struct ChattingRoomNameData
{
	WCHAR name[20];
	WCHAR roomUserNumInfo[4];
	bool pwInfo;
	ChattingRoomNameData()
	{
		wcscpy(name, L"");
		wcscpy(roomUserNumInfo, L"");
		pwInfo = FALSE;
	}
};
//ä�ù� ��� ����Ʈ�ڽ��� ��Ŷ ����
struct ChattingRoomName_NF
{
	Packet chattingRoomNameHeader;
	ChattingRoomNameData data;
	ChattingRoomName_NF()
	{
		chattingRoomNameHeader.header = CHATTING_ROOM_NAME_NF;
	}
};

/////////////////////////////////////////////////////////////////////
/*
ä�ù� �����û�� Ŭ�� ID�� ������ ������ ��Ŷ
*/
//ä�ù濡 ���� ��û Ŭ�� ������ (chattingRoomName, loginID)
struct ChatEnterData
{
	WCHAR chattingRoomName[20];
	WCHAR loginID[10];
	ChatEnterData()
	{
		wcscpy(chattingRoomName, L"");
		wcscpy(loginID, L"");
	}
};
//[Ŭ�� -> ����] ä�ù濡 ���� ��û ��Ŷ ����
struct ChatEnter_RQ
{
	Packet chatEnterHeader;
	ChatEnterData chatEnterData;
	ChatEnter_RQ()
	{
		chatEnterHeader.header = CHATTING_ROOM_ENTER_RQ;
		chatEnterHeader.dataSize = sizeof(ChatEnterData);
	}
};
//���� ��û ��� Ȯ�� ������ (roomName, success)
struct ChatEnterData_RP
{
	WCHAR roomName[20];
	bool havePW;
	bool success;
	ChatEnterData_RP()
	{
		wcscpy(roomName, L"");
		havePW = FALSE;
		success = FALSE;
	}
};
//[���� -> Ŭ��]���� ��û ��� ��Ŷ ����
struct ChatEnter_RP
{
	Packet chatEnterHeader;
	ChatEnterData_RP data;
	ChatEnter_RP()
	{
		chatEnterHeader.header = CHATTING_ROOM_ENTER_RP;
		chatEnterHeader.dataSize = sizeof(ChatEnterData_RP);
	}
};
/////////////////////////////////////////////////////////////////////
/*
��й�ȣ Ȯ�� ��Ŷ
*/
//��й�ȣ ���� ������ (roomName, pw)
struct CheckChatPwData_RQ
{
	WCHAR roomName[20];
	WCHAR id[10];
	WCHAR pw[10];
	CheckChatPwData_RQ()
	{
		wcscpy(roomName, L"");
		wcscpy(id, L"");
		wcscpy(pw, L"");
	}
};
//[Ŭ�� -> ����] ä�ù��� ���� Ŭ�� ���� ��Ŷ ����
struct CheckChatPw_RQ
{
	Packet chatOutHeader;
	CheckChatPwData_RQ data;
	CheckChatPw_RQ()
	{
		chatOutHeader.header = CHECK_CHAT_PW_RQ;
		chatOutHeader.dataSize = sizeof(ChatEnterData);
	}
};
////////////////////////////////////////////////////
//[���� -> Ŭ��] ä�ù� ���� Ŭ�� Ȯ�� ��Ŷ ����
struct CheckChatPw_RP
{
	Packet chatOutHeader;
	bool success;
	CheckChatPw_RP()
	{
		chatOutHeader.header = CHECK_CHAT_PW_RP;
		chatOutHeader.dataSize = sizeof(bool);
		success = FALSE;
	}
};

/////////////////////////////////////////////////////////////////////
/*
ä�ù��� ���� Ŭ���̾�Ʈ�� ������ ������ ��Ŷ
*/
//ä�ù��� ���� Ŭ�� ������ (id, roomName)
struct ChatOutData_RQ
{
	WCHAR id[10];
	WCHAR roomName[20];
	ChatOutData_RQ()
	{
		wcscpy(id, L"");
		wcscpy(roomName, L"");
	}
};
//[Ŭ�� -> ����] ä�ù��� ���� Ŭ�� ���� ��Ŷ ����
struct ChatOut_RQ
{
	Packet chatOutHeader;
	ChatOutData_RQ data;
	ChatOut_RQ()
	{
		chatOutHeader.header = CHATTING_ROON_OUT_RQ;
		chatOutHeader.dataSize = sizeof(ChatEnterData);
	}
};
//[���� -> Ŭ��] ä�ù� ���� Ŭ�� Ȯ�� ��Ŷ ����
struct ChatOut_RP
{
	Packet chatOutHeader;
	bool success;
	ChatOut_RP()
	{
		chatOutHeader.header = CHATTING_ROON_OUT_RP;
		chatOutHeader.dataSize = sizeof(bool);
		success = FALSE;
	}
};
/////////////////////////////////////////////////////////////////////
/*
ä�ù��� ���� Ŭ�� ���� 0���̸� ä�ù� ���ִ� ��Ŷ
*/
//ä�ù� ���Ÿ� ���� ������ (roomName)
struct ChatRoomDeleteData
{
	WCHAR roomName[20];
	ChatRoomDeleteData()
	{
		wcscpy(roomName, L"");
	}

};
//[���� -> Ŭ��] ��� Ŭ�� ä�ù� ���� ��û ��Ŷ ����
struct ChatRoomDelete_NF
{
	Packet chatRoomDeleteHeader;
	ChatRoomDeleteData data;
	ChatRoomDelete_NF()
	{
		chatRoomDeleteHeader.header = CHATTING_ROOM_DELETE_NF;
		chatRoomDeleteHeader.dataSize = sizeof(ChatRoomDeleteData);
	}

};
/////////////////////////////////////////////////////////////////////
/*
ä�ù濡�� ���ڿ� �����ϴ� ��Ŷ
*/
//���ڿ� ������ (chatRoomName, str)
struct StrData_RQ
{
	WCHAR chatRoomName[20];
	WCHAR str[30];
	StrData_RQ()
	{
		wcscpy(chatRoomName, L"");
		wcscpy(str, L"");
	}
};
//[Ŭ��->����] ä�ù濡�� Ŭ���� ���ڿ� ���� ��û ��Ŷ
struct ChatStrData_RQ
{
	Packet chatHeader;
	StrData_RQ chatData;
	ChatStrData_RQ()
	{
		chatHeader.header = CHAT_STR_DATA_RQ;
		chatHeader.dataSize = sizeof(StrData_RQ);
	}
};
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/*
ä�ù濡 ���ڿ� �ѷ��ִ� ��Ŷ
*/
//ä�ù濡 ���ڿ� �ѷ��ִ� ������ (str)
struct StrData_RP
{
	WCHAR str[30];
	StrData_RP()
	{
		wcscpy(str, L"");
	}
};
//ä�ù濡 ���ڿ� �ѷ��ִ� ��Ŷ
struct ChatStrData_RP
{
	Packet chatHeader;
	StrData_RP chatData;
	ChatStrData_RP()
	{
		chatHeader.header = CHAT_STR_DATA_RP;
		chatHeader.dataSize = sizeof(StrData_RP);
	}
};
/////////////////////////////////////////////////////////////////////