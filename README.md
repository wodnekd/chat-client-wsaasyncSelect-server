# chat-client-wsaasyncSelect-server
-----
![image](https://user-images.githubusercontent.com/86718283/124404333-7348e000-dd75-11eb-8377-24e6cebbadcc.png)

----------------
>사용언어
```
c / c ++ / win32
```
----------------
>서버
```
WSAAsyncselect server
```
+ ClientManager.h
+ Database.h
+ LoginDatabase.h
+ protocal.h
+ Resource.h
+ stdafx.h
+ WSAAsyncSelect_Server.cpp

>>InitServer


![image](https://user-images.githubusercontent.com/86718283/124505997-b3f03a00-de05-11eb-9d15-21413b668f48.png)
1. 윈속초기화
2. Listen, bind
3. WSAAsyncSelect

>>WM_SOCKET 발생시


![image](https://user-images.githubusercontent.com/86718283/124506175-0fbac300-de06-11eb-926b-27124a3f5994.png)

>>ProcessSocketMessage 에서 처리


![image](https://user-images.githubusercontent.com/86718283/124506396-80fa7600-de06-11eb-948e-926cde422462.png)
![image](https://user-images.githubusercontent.com/86718283/124506410-8eaffb80-de06-11eb-8c54-8fdc5c66699a.png)

----------------
>클라이언트
```
win32
```
+ chatting_client.h
+ Protocal.h
+ Resource.h
+ sendFunc.h
+ stdafx.h

>> 로그인 구현


![image](https://user-images.githubusercontent.com/86718283/124506529-cae35c00-de06-11eb-81da-05f7a5f18adb.png)
1. 로그인 구현
2. 회원가입 구현
3. pw 찾기 구현

>> 메신저 대기창 구현


![image](https://user-images.githubusercontent.com/86718283/124506753-4a712b00-de07-11eb-93c0-721cd8769a19.png)
1. 나의 nickname 표현
2. 친구목록 구현 (로그인한 유저 nickname)
3. 쪽지보내기 구현
4. 답장하기 구현

>> 메신저 대화방 구현


![image](https://user-images.githubusercontent.com/86718283/124506832-712f6180-de07-11eb-8dab-2e02a5249c67.png)
1. 비밀방 생성 구현
2. 인원 설정 구현
3. 채팅방 구현
-----------------




