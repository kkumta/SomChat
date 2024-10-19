#include "ClientPacketHandler.h"
#include "pch.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO: Log

	return false;
}

bool Handle_S_ENTER_ROOM(PacketSessionPtr& session, Protocol::S_ENTER_ROOM& pkt)
{
	wcout << Utils::StringToWString(pkt.userinfo().nickname()).c_str() << L"님이 방에 입장했습니다.\n";

	return true;
}

bool Handle_S_LEAVE_ROOM(PacketSessionPtr& session, Protocol::S_LEAVE_ROOM& pkt)
{
	wcout << Utils::StringToWString(pkt.nickname()).c_str() << L"님이 방에서 퇴장했습니다.\n";

	return true;
}

bool Handle_S_CHAT(PacketSessionPtr& session, Protocol::S_CHAT& pkt)
{
	wcout << Utils::StringToWString(pkt.userinfo().nickname()).c_str() << ": " << Utils::StringToWString(pkt.msg()).c_str() << '\n';

	return true;
}