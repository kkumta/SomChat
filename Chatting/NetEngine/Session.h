#pragma once

#include "Macro.h"
#include "Iocp.h"
#include "IocpObject.h"
#include "IocpEvent.h"
#include "SockAddress.h"
#include "RecvBuffer.h"

class Service;

class Session : public IocpObject
{
	friend class Listener;

	enum
	{
		BUFFER_SIZE = 0x10000,
	};

private:
	USE_LOCK;

public:
	Session();
	virtual ~Session();

	// �ٸ� Ŭ�������� ����ϴ� �Լ�
	bool Connect();
	void Disconnect(const WCHAR* cause);
	void Send(SendBufferPtr sendBuffer);

	// Get, Set
	ServicePtr GetService() { return _service.lock(); }
	void SetService(ServicePtr service) { _service = service; }

	SOCKET GetSocket() { return _socket; }
	SessionPtr GetSessionPtr() { return static_pointer_cast<Session>(shared_from_this()); }
	bool IsConnected() { return _isConnected; }

private:
	// IocpObject�� �Լ� ����
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0);

	// Iocp Register, Process
	bool RegisterConnect();
	void ProcessConnect();

	bool RegisterDisconnect();
	void ProcessDisconnect();

	void RegisterSend();
	void ProcessSend(int32 numOfBytes); // ������ ó���� �����Ͱ� �ִ� ��쿡�� ȣ��

	void RegisterRecv();
	void ProcessRecv(int32 numOfBytes);

	void HandleError(int32 errorCode);

protected:
	// Session�� ��ӹ޴� Ŭ�������� ������ �Լ�
	virtual void OnConnected() {}
	virtual void OnDisconnected() {}
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) {}

private:
	// ���� ���� ����
	weak_ptr<Service> _service;
	SOCKET _socket = INVALID_SOCKET;
	atomic<bool> _isConnected = false;

	// �ۼ��� ���� ����
	queue<SendBufferPtr> _sendBuffers;
	atomic<bool> _sendRegistered = false;
	RecvBuffer _recvBuffer;

	// IocpEvent ���� ����
	ConnectEvent _connectEvent;
	DisconnectEvent _disconnectEvent;
	RecvEvent _recvEvent;
	SendEvent _sendEvent;
};

struct PacketHeader
{
	uint16 size;
	uint16 id;
};

class PacketSession : public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionPtr GetPacketSessionPtr() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32		OnRecv(BYTE* buffer, int32 len) sealed;
	virtual void		OnRecvPacket(BYTE* buffer, int32 len) abstract;
};