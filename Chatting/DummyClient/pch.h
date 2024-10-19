#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "NetEngine\\Debug\\NetEngine.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "NetEngine\\Release\\NetEngine.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"
#include "ClientPacketHandler.h"
#include "ChatSession.h"
#include "Utils.h"

USING_SHARED_PTR(ChatSession);
USING_SHARED_PTR(User);
USING_SHARED_PTR(Room);