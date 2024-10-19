#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

	bool HandleEnterUser(UserPtr player);
	bool HandleLeaveUser(UserPtr player);
	bool HandleChatUser(UserPtr user, string msg);

	// Obejct가 Room에서 하는 행위 관련
	bool EnterRoom(UserPtr user);
	bool LeaveRoom(UserPtr user);
	bool ChatRoom(UserPtr user, string msg);

	RoomPtr GetRoomPtr();

private:
	bool AddUser(UserPtr user);
	bool RemoveUser(uint64 userId);

private:
	void Broadcast(SendBufferPtr sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, UserPtr> _users;
};

extern RoomPtr GRoom;