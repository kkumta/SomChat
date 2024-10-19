#pragma once

#include "Types.h"
#include "Macro.h"
#include "CoreGlobal.h"
#include "TLS.h"

#include <array>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <windows.h>
#include <iostream>
#include <assert.h>
using namespace std;

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "SendBuffer.h"
#include "RecvBuffer.h"
#include "Service.h"
#include "Session.h"
#include "JobQueue.h"