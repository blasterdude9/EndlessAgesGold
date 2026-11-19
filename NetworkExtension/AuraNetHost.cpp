//======================================================================
// AuraNetHost.cpp  -  ENet-backed transport wrapper (replaces DirectPlay8)
//======================================================================
#include "stdafx.h"
#include "AuraNetHost.h"
#include "enet/enet.h"
#include <string.h>
#include <windows.h>

// Scoped lock on the host's critical section (recursive, so a handler that
// sends from inside the pump is fine).
struct AuraNetGuard
{
	CRITICAL_SECTION * cs;
	AuraNetGuard(void * p) : cs((CRITICAL_SECTION *)p) { if (cs) EnterCriticalSection(cs); }
	~AuraNetGuard() { if (cs) LeaveCriticalSection(cs); }
};

// ENet must be initialised once per process. Refcount so multiple hosts are safe.
static int g_enetRefCount = 0;
static bool EnsureEnetInit()
{
	if (g_enetRefCount == 0)
	{
		if (enet_initialize() != 0)
			return false;
	}
	g_enetRefCount++;
	return true;
}
static void ReleaseEnet()
{
	if (g_enetRefCount > 0)
	{
		g_enetRefCount--;
		if (g_enetRefCount == 0)
			enet_deinitialize();
	}
}

AuraNetHost::AuraNetHost()
	: m_host(0), m_isServer(false), m_nextPlayerID(2), m_serverPeer(0),
	  m_rxBuffer(0), m_rxBufferCap(0), m_lock(0)
{
	CRITICAL_SECTION * cs = new CRITICAL_SECTION;
	InitializeCriticalSectionAndSpinCount(cs, 4000);
	m_lock = cs;
}

AuraNetHost::~AuraNetHost()
{
	Shutdown();
	if (m_rxBuffer) { free(m_rxBuffer); m_rxBuffer = 0; }
	if (m_lock)
	{
		DeleteCriticalSection((CRITICAL_SECTION *)m_lock);
		delete (CRITICAL_SECTION *)m_lock;
		m_lock = 0;
	}
}

//----------------------------------------------------------------------
// id <-> peer mapping (reproduces the DPNID player-id model)
//----------------------------------------------------------------------
AuraNetID AuraNetHost::RegisterPeer(ENetPeer * peer)
{
	// Player ids start at 2: 0 means "none" and 1 is DPID_SERVERPLAYER, which the
	// game code uses to mean "the server itself" (SendAttribMsg etc. route id 1
	// to a server-side outbox that only exists on the client - a player with
	// id 1 crashed the server on its first spawn message).
	AuraNetID id = m_nextPlayerID++;
	if (m_nextPlayerID < 2) m_nextPlayerID = 2;   // never hand out 0 or 1 (wrap)
	m_idToPeer[id] = peer;
	m_peerToId[peer] = id;
	peer->data = (void *)(size_t)id;               // fast reverse lookup
	return id;
}
AuraNetID AuraNetHost::IdForPeer(ENetPeer * peer)
{
	if (peer && peer->data) return (AuraNetID)(size_t)peer->data;
	std::map<ENetPeer *, AuraNetID>::iterator it = m_peerToId.find(peer);
	return (it != m_peerToId.end()) ? it->second : 0;
}
ENetPeer * AuraNetHost::PeerForId(AuraNetID id)
{
	std::map<AuraNetID, ENetPeer *>::iterator it = m_idToPeer.find(id);
	return (it != m_idToPeer.end()) ? it->second : 0;
}
void AuraNetHost::ForgetPeer(ENetPeer * peer)
{
	AuraNetID id = IdForPeer(peer);
	if (id) m_idToPeer.erase(id);
	m_peerToId.erase(peer);
	if (peer) peer->data = 0;
}

//----------------------------------------------------------------------
// setup
//----------------------------------------------------------------------
bool AuraNetHost::StartServer(unsigned short port, int maxPlayers)
{
	AuraNetGuard guard(m_lock);
	if (!EnsureEnetInit()) return false;
	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = port;
	m_host = enet_host_create(&address, maxPlayers, 2 /*channels*/, 0, 0);
	if (!m_host) { ReleaseEnet(); return false; }
	m_isServer = true;
	return true;
}

bool AuraNetHost::StartClient()
{
	AuraNetGuard guard(m_lock);
	if (!EnsureEnetInit()) return false;
	m_host = enet_host_create(0, 1, 2 /*channels*/, 0, 0);
	if (!m_host) { ReleaseEnet(); return false; }
	m_isServer = false;
	return true;
}

AuraNetID AuraNetHost::Connect(const char * ip, unsigned short port, unsigned int timeoutMs)
{
	AuraNetGuard guard(m_lock);
	if (!m_host) { if (!StartClient()) return 0; }
	ENetAddress address;
	if (enet_address_set_host(&address, ip) != 0) return 0;
	address.port = port;

	ENetPeer * peer = enet_host_connect(m_host, &address, 2, 0);
	if (!peer) return 0;

	// Wait for the connection to be acknowledged.
	ENetEvent ev;
	if (enet_host_service(m_host, &ev, timeoutMs) > 0 && ev.type == ENET_EVENT_TYPE_CONNECT)
	{
		m_serverPeer = peer;
		return RegisterPeer(peer);
	}
	enet_peer_reset(peer);
	return 0;
}

//----------------------------------------------------------------------
// pump
//----------------------------------------------------------------------
bool AuraNetHost::PollEvent(AuraNetEvent & out, unsigned int timeoutMs)
{
	AuraNetGuard guard(m_lock);
	out.type = ANET_NONE; out.playerID = 0; out.data = 0; out.size = 0;
	if (!m_host) return false;

	ENetEvent ev;
	int r = enet_host_service(m_host, &ev, timeoutMs);
	if (r <= 0) return false;

	switch (ev.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
		out.type = ANET_CONNECT;
		out.playerID = RegisterPeer(ev.peer);
		return true;

	case ENET_EVENT_TYPE_RECEIVE:
	{
		out.type = ANET_RECEIVE;
		out.playerID = IdForPeer(ev.peer);
		unsigned long len = (unsigned long)ev.packet->dataLength;
		if (len > m_rxBufferCap)
		{
			unsigned char * nb = (unsigned char *)realloc(m_rxBuffer, len);
			if (nb) { m_rxBuffer = nb; m_rxBufferCap = len; }
		}
		if (m_rxBuffer && len <= m_rxBufferCap)
		{
			memcpy(m_rxBuffer, ev.packet->data, len);
			out.data = m_rxBuffer;
			out.size = len;
		}
		enet_packet_destroy(ev.packet);
		return true;
	}

	case ENET_EVENT_TYPE_DISCONNECT:
		out.type = ANET_DISCONNECT;
		out.playerID = IdForPeer(ev.peer);
		ForgetPeer(ev.peer);
		if (ev.peer == m_serverPeer) m_serverPeer = 0;
		return true;

	default:
		return false;
	}
}

//----------------------------------------------------------------------
// send
//----------------------------------------------------------------------
bool AuraNetHost::Send(AuraNetID playerID, const unsigned char * data, unsigned long size, bool reliable)
{
	AuraNetGuard guard(m_lock);
	if (!m_host || !data || size == 0) return false;

	ENetPeer * peer = 0;
	if (m_isServer)
		peer = PeerForId(playerID);
	else
		peer = m_serverPeer;        // client always sends to the server
	if (!peer) return false;

	ENetPacket * packet = enet_packet_create(data, size,
		reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
	if (!packet) return false;

	return enet_peer_send(peer, 0 /*channel*/, packet) == 0;
}

//----------------------------------------------------------------------
// teardown / info
//----------------------------------------------------------------------
void AuraNetHost::DisconnectPlayer(AuraNetID playerID)
{
	AuraNetGuard guard(m_lock);
	ENetPeer * peer = PeerForId(playerID);
	if (peer) enet_peer_disconnect(peer, 0);
}

void AuraNetHost::Shutdown()
{
	AuraNetGuard guard(m_lock);
	if (m_host)
	{
		enet_host_destroy(m_host);
		m_host = 0;
		ReleaseEnet();
	}
	m_idToPeer.clear();
	m_peerToId.clear();
	m_serverPeer = 0;
}

bool AuraNetHost::GetPlayerIP(AuraNetID playerID, char * outBuf, int bufLen)
{
	AuraNetGuard guard(m_lock);
	ENetPeer * peer = PeerForId(playerID);
	if (!peer || !outBuf || bufLen <= 0) return false;
	return enet_address_get_host_ip(&peer->address, outBuf, bufLen) == 0;
}
