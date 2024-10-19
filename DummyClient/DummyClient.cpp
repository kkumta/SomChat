#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"

int main()
{
	ClientPacketHandler::Init();

	this_thread::sleep_for(1s);

	ClientServicePtr service = make_shared<ClientService>(
		SockAddress(L"127.0.0.1", 7777),
		make_shared<Iocp>(),
		[=]() { return make_shared<ChatSession>(); },
		1);

	ASSERT_CRASH(service->Start());

	// 스레드 개수는 코어 개수~코어 개수 * 1.5가 적당하다
	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]()
		{
			while (true)
			{
				service->GetIocp()->Dispatch();
			}
		});
	}

	// 로케일 설정
	locale::global(locale("ko_KR.UTF-8"));
	wcin.imbue(locale(""));

	// 닉네임 설정 및 방 입장
	{
		wcout << L"채팅에 사용할 닉네임을 입력하세요: ";
		wstring nickname;
		getline(wcin, nickname);

		Protocol::C_ENTER_ROOM enterRoomPkt;
		enterRoomPkt.set_nickname(Utils::WStringToString(nickname).c_str());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterRoomPkt);
		service->GetSession()->Send(sendBuffer);
	}

	// 채팅 보내기
	while (true)
	{
		wstring msg;
		getline(wcin, msg);

		Protocol::C_CHAT chatPkt;
		chatPkt.set_msg(Utils::WStringToString(msg).c_str());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);
		service->GetSession()->Send(sendBuffer);
	}

	GThreadManager->Join();
}