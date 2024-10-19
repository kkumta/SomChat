#include "pch.h"
#include "ChatSession.h"
#include "ClientPacketHandler.h"
#include "ChatSession.h"

void ChatSession::OnConnected()
{
}

void ChatSession::OnDisconnected()
{
}

void ChatSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionPtr session = GetPacketSessionPtr();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void ChatSession::OnSend(int32 len)
{
}