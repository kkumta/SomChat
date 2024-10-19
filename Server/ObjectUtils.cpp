#include "pch.h"
#include "ObjectUtils.h"
#include "User.h"
#include "ChatSession.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

UserPtr ObjectUtils::CreateUser(ChatSessionPtr session, string nickname)
{
	const int64 newId = s_idGenerator.fetch_add(1);

	UserPtr user = make_shared<User>();
	user->userInfo->set_user_id(newId);
	user->userInfo->set_nickname(nickname);

	user->session = session;
	session->user.store(user);

	return user;
}