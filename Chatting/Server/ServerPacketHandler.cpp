#include "pch.h"
#include "ServerPacketHandler.h"
#include "Room.h"
#include "ChatSession.h"
#include "User.h"
#include "ObjectUtils.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionPtr& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_C_ENTER_ROOM(PacketSessionPtr& session, Protocol::C_ENTER_ROOM& pkt)
{
	UserPtr user = ObjectUtils::CreateUser(static_pointer_cast<ChatSession>(session), pkt.nickname());

	// 플레이어를 방에 입장시킨다.
	GRoom->DoAsync(&Room::HandleEnterUser, user);

	return false;
}

bool Handle_C_LEAVE_ROOM(PacketSessionPtr& session, Protocol::C_LEAVE_ROOM& pkt)
{
	auto chatSession = static_pointer_cast<ChatSession>(session);

	UserPtr user = chatSession->user.load();
	if (user == nullptr)
		return false;

	RoomPtr room = user->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleLeaveUser, user);

	return true;
}

bool Handle_C_CHAT(PacketSessionPtr& session, Protocol::C_CHAT& pkt)
{
	auto chatSession = static_pointer_cast<ChatSession>(session);

	UserPtr user = chatSession->user.load();
	if (user == nullptr)
		return false;

	RoomPtr room = user->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&Room::HandleChatUser, user, pkt.msg());

	return true;
}