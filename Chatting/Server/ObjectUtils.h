#pragma once

class ObjectUtils
{
public:
	static UserPtr CreateUser(ChatSessionPtr session, string nickname);

private:
	static atomic<int64> s_idGenerator;
};