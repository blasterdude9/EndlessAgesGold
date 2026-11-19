//======================================================================
// AuraNetHost.h
//
// Thin transport wrapper around ENet (modern reliable-UDP), replacing the
// dead DirectPlay8 layer. This is the ONLY place in the engine that talks
// to the networking library, so if the transport is ever changed again it
// is one file, not a hunt through the codebase.
//
// Players are identified by a plain number (AuraNetID / DPNID) exactly as
// DirectPlay did, so the game code above this layer is unchanged.
//======================================================================
#pragma once

#include <map>

struct _ENetHost;
struct _ENetPeer;

typedef unsigned long AuraNetID;   // same meaning as the old DPNID

enum AuraNetEventType
{
	ANET_NONE = 0,
	ANET_CONNECT,       // a player connected      (playerID valid)
	ANET_RECEIVE,       // data arrived            (playerID, data, size valid)
	ANET_DISCONNECT     // a player disconnected   (playerID valid)
};

struct AuraNetEvent
{
	AuraNetEventType type;
	AuraNetID        playerID;
	unsigned char *  data;   // valid only until the next PollEvent() call
	unsigned long    size;
};

class AuraNetHost
{
public:
	AuraNetHost();
	~AuraNetHost();

	// --- setup ---
	bool StartServer(unsigned short port, int maxPlayers);   // host a game
	bool StartClient();                                      // create a client host
	// Connect to a server by IP; returns the server's assigned playerID (or 0 on failure).
	AuraNetID Connect(const char * ip, unsigned short port, unsigned int timeoutMs);

	// --- per-frame pump: drain one event. Returns true if an event was filled. ---
	bool PollEvent(AuraNetEvent & outEvent, unsigned int timeoutMs);

	// --- send bytes to a player (reliable == old DPNSEND_GUARANTEED) ---
	bool Send(AuraNetID playerID, const unsigned char * data, unsigned long size, bool reliable);

	// --- teardown / info ---
	void DisconnectPlayer(AuraNetID playerID);
	void Shutdown();
	bool GetPlayerIP(AuraNetID playerID, char * outBuf, int bufLen);
	bool IsServer() const { return m_isServer; }
	bool IsActive() const { return m_host != 0; }

private:
	AuraNetID RegisterPeer(_ENetPeer * peer);     // assign an id to a new peer
	AuraNetID IdForPeer(_ENetPeer * peer);
	_ENetPeer * PeerForId(AuraNetID id);
	void ForgetPeer(_ENetPeer * peer);

	_ENetHost * m_host;
	bool        m_isServer;
	AuraNetID   m_nextPlayerID;
	_ENetPeer * m_serverPeer;   // client side: the one connection to the server

	std::map<AuraNetID, _ENetPeer *> m_idToPeer;
	std::map<_ENetPeer *, AuraNetID> m_peerToId;

	unsigned char * m_rxBuffer;     // reused receive buffer handed out via events
	unsigned long   m_rxBufferCap;

	// ENet is not thread-safe. The game server and AI server call Send() from
	// many worker threads while the network thread runs PollEvent(); without
	// this lock the peer queues get corrupted and connections drop or crash.
	void *          m_lock;         // CRITICAL_SECTION* (opaque here)
};
