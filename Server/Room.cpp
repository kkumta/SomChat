#include "pch.h"
#include "Room.h"
#include "User.h"
#include "ChatSession.h"
#include "ObjectUtils.h"

RoomPtr GRoom = make_shared<Room>();

Room::Room()
{

}

Room::~Room()
{

}

bool Room::EnterRoom(UserPtr user)
{
	bool success = AddUser(user);

	// 입장 사실을 모든 유저에게 알린다
	Protocol::S_ENTER_ROOM enterRoomPkt;
	enterRoomPkt.set_success(success);

	Protocol::UserInfo* userInfo = new Protocol::UserInfo();
	userInfo->CopyFrom(*user->userInfo);
	enterRoomPkt.set_allocated_userinfo(userInfo);

	SendBufferPtr sendBuffer = ServerPacketHandler::MakeSendBuffer(enterRoomPkt);
	Broadcast(sendBuffer, 0);

	return success;
}

bool Room::LeaveRoom(UserPtr user)
{
	if (user == nullptr)
		return false;

	const uint64 userId = user->userInfo->user_id();
	const string userNickname = user->userInfo->nickname();
	bool success = RemoveUser(userId);

	// 퇴장 사실을 방에 남아있는 모든 유저에게 알린다
	{
		Protocol::S_LEAVE_ROOM leaveRoomPkt;
		leaveRoomPkt.set_nickname(userNickname);

		SendBufferPtr sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveRoomPkt);
		Broadcast(sendBuffer, userId);
	}

	return success;
}

bool Room::ChatRoom(UserPtr user, string msg)
{
	bool success = true;

	if (user == nullptr)
		return false;

	// 나 자신을 제외한 모든 유저에게 채팅을 보낸다
	{
		Protocol::S_CHAT chatPkt;
		chatPkt.set_success(success);
		Protocol::UserInfo* userInfo = chatPkt.mutable_userinfo();
		userInfo->CopyFrom(*user->userInfo);
		chatPkt.set_msg(msg);

		SendBufferPtr sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);
		Broadcast(sendBuffer, user->userInfo->user_id());
	}

	return success;
}

bool Room::HandleEnterUser(UserPtr user)
{
	return EnterRoom(user);
}

bool Room::HandleLeaveUser(UserPtr user)
{
	return LeaveRoom(user);
}

bool Room::HandleChatUser(UserPtr user, string msg)
{
	return ChatRoom(user, msg);
}

RoomPtr Room::GetRoomPtr()
{
	return static_pointer_cast<Room>(shared_from_this());
}

bool Room::AddUser(UserPtr user)
{
	// 있다면 문제가 있다.
	if (_users.find(user->userInfo->user_id()) != _users.end())
		return false;

	_users.insert(make_pair(user->userInfo->user_id(), user));

	user->room.store(GetRoomPtr());

	return true;
}

bool Room::RemoveUser(uint64 userId)
{
	// 없다면 문제가 있다.
	if (_users.find(userId) == _users.end())
		return false;

	UserPtr user = _users[userId];
	if (user)
		user->room.store(weak_ptr<Room>());

	_users.erase(userId);

	return true;
}

void Room::Broadcast(SendBufferPtr sendBuffer, uint64 exceptId)
{
	for (auto& item : _users)
	{
		UserPtr user = dynamic_pointer_cast<User>(item.second);
		if (user == nullptr)
			continue;
		if (user->userInfo->user_id() == exceptId)
			continue;

		if (ChatSessionPtr session = user->session.lock())
			session->Send(sendBuffer);
	}
}