#include "pch.h"
#include <iostream>
#include "ThreadManager.h"

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServicePtr& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocp()->Dispatch(10);

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork(THREAD_TYPE::LOGIC);
	}
}

int main()
{
	ServerPacketHandler::Init();

	ServerServicePtr service = make_shared<ServerService>(
		SockAddress(L"127.0.0.1", 7777),
		make_shared<Iocp>(),
		[=]() { return make_shared<ChatSession>(); },
		100);

	ASSERT_CRASH(service->Start());

	// 게임 로직 스레드
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
		{
			DoWorkerJob(service);
		});
	}

	GThreadManager->Join();
}