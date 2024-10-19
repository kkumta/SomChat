#include "pch.h"
#include "ChatSession.h"
#include "ServerPacketHandler.h"
#include "User.h"
#include "Room.h"

void ChatSession::OnConnected()
{
}

void ChatSession::OnDisconnected()
{
	ChatSessionPtr chatSession = static_pointer_cast<ChatSession>(GetPacketSessionPtr());

	UserPtr user = chatSession->user.load();

	RoomPtr room = user->room.load().lock();

	room->DoAsync(&Room::HandleLeaveUser, user);
}

void ChatSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionPtr session = GetPacketSessionPtr();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void ChatSession::OnSend(int32 len)
{
}