#pragma once
#include <windows.h>
/*
Request == 클라 -> 서버에 요청
Reply	== 서버 -> 클라에 응답
Notify	== 서버 -> 클라에 통보 (다른 클라의 행동을 보낼 때)
Command == 서버 -> 클라에 명령 (서버에 의해 클라 수정될 때)

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
헤더와 헤더 다음 데이터의 사이즈를 저장하는 패킷
size = 8byte;
*/
struct Packet
{
	int header;
	int dataSize;
};

/////////////////////////////////////////////////////////////////////
/*
회원가입 하는 패킷
*/
//회원가입 데이터 (id, pw, nickname, quest) 
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
//[클라 -> 서버] id, pw, nickname, 질문 패킷 전송
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
//[서버 -> 클라] 회원가입 성공 여부 담은 패킷 전송
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
접속 된 다른 클라이언트 ID를 알려주는 패킷
*/
//아이디 알려주는 데이터 (id)
struct OtherIdInData
{
	WCHAR otherID[10];
	OtherIdInData()
	{
		wcscpy(otherID, L"");
	}
};
//[서버 -> 클라] 접속 되어 있는 클라 모두에게 패킷 전송
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
종료 한 다른 클라이언트 ID를 알려주는 패킷
*/
// 아이디 알려주는 데이터 (id)
struct OtherIdOutData
{
	WCHAR otherID[10];
	OtherIdOutData()
	{
		wcscpy(otherID, L"");
	}
};
//[서버 -> 클라] 접속 되어 있는 클라 모드에게 패킷 전송
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
회원 가입 시 아이디 확인 패킷
*/
// 아이디 확인 데이터 (id)
struct CompareData
{
	WCHAR compareChar[10];
	CompareData()
	{
		wcscpy(compareChar, L"");
	}
};
//[클라 -> 서버] 아이디 중복 확인 패킷 전송
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
//[서버 -> 클라] 아이디 중복 확인 결과 패킷 전송
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

//[클라 -> 서버] 닉네임 중복 확인 패킷 전송
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
//[서버 -> 클라] 아이디 중복 확인 결과 패킷 전송
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
비밀번호 찾는 패킷
*/
//비밀번호 찾기 데이터 (id, quest)
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
//[클라 -> 서버] 비밀번호 찾기 위해 보내는 패킷
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
//비밀번호 데이터 (pw, success)
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
//[서버 -> 클라] 비밀번호 찾기 성공 실패 확인 후 보내는 패킷 
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
로그인 시 아이디 비밀번호 확인하는 패킷
*/
//로그인 데이터 (id, pw)
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
//[클라 -> 서버] 로그인 확인하기 위해 보내는 패킷 
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

//로그인 확인 후 성공 여부 데이터 (nickname, success)
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
//[서버 -> 클라] 로그인 확인 후 성공 여부 보내는 패킷
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
쪽지 보내기 1:1 문자열 보내는 패킷
*/
// 쪽지 보내는 데이터 (id, str)
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
//[클라 -> 서버] 1:1 문자열 전송 패킷
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
//[서버 -> 클라] 문자열 전송 성공 여부 확인 패킷
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
채팅방 생성 요청 보내는 패킷
*/
//채팅방 생성에 필요한 정보 데이터 (name, pw, roomSize)
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
//[클라 -> 서버] 채팅방 생성 요청 패킷 전송
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
//채팅방 생성 완료시 전송되는 데이터 (name, roomUserNumInfo, success)
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
//[서버 -> 클라] 채팅방 생성 성공 여부 패킷 전송
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
메인 창 채팅 리스트박스에 인원수 뿌려주는 패킷
*/

//채팅방 입장시 
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
메인 창 채팅 리스트박스에 방제 뿌려주는 패킷
*/
//채팅방 목록 리스트 박스에 들어갈 데이터 (name, roomUserNumInfo, pwInfo)
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
//채팅방 목록 리스트박스에 패킷 전송
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
채팅방 입장요청한 클라 ID를 서버에 보내는 패킷
*/
//채팅방에 입장 요청 클라 데이터 (chattingRoomName, loginID)
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
//[클라 -> 서버] 채팅방에 입장 요청 패킷 전송
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
//입장 요청 결과 확인 데이터 (roomName, success)
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
//[서버 -> 클라]입장 요청 결과 패킷 전송
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
비밀번호 확인 패킷
*/
//비밀번호 전송 데이터 (roomName, pw)
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
//[클라 -> 서버] 채팅방을 나간 클라 정보 패킷 전송
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
//[서버 -> 클라] 채팅방 나간 클라 확인 패킷 전송
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
채팅방을 나간 클라이언트를 서버에 보내는 패킷
*/
//채팅방을 나간 클라 데이터 (id, roomName)
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
//[클라 -> 서버] 채팅방을 나간 클라 정보 패킷 전송
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
//[서버 -> 클라] 채팅방 나간 클라 확인 패킷 전송
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
채팅방을 나간 클라 수가 0명이면 채팅방 없애는 패킷
*/
//채팅방 제거를 위한 데이터 (roomName)
struct ChatRoomDeleteData
{
	WCHAR roomName[20];
	ChatRoomDeleteData()
	{
		wcscpy(roomName, L"");
	}

};
//[서버 -> 클라] 모든 클라에 채팅방 제거 요청 패킷 전송
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
채팅방에서 문자열 전송하는 패킷
*/
//문자열 데이터 (chatRoomName, str)
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
//[클라->서버] 채팅방에서 클라의 문자열 전송 요청 패킷
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
채팅방에 문자열 뿌려주는 패킷
*/
//채팅방에 문자열 뿌려주는 데이터 (str)
struct StrData_RP
{
	WCHAR str[30];
	StrData_RP()
	{
		wcscpy(str, L"");
	}
};
//채팅방에 문자열 뿌려주는 패킷
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