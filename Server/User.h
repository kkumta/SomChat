#pragma once

class ChatSession;
class Room;

class User
{
public:
	User();
	~User();

public:
	Protocol::UserInfo* userInfo;
	atomic<weak_ptr<Room>> room;
	weak_ptr<ChatSession> session;
};