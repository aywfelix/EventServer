#pragma  once

#include <unordered_map>
#include <functional>
#include <memory>
#include "Packet.h"

class Player;

typedef std::function<int(Player*, Packet*)> MsgHandle;

class PacketMgr
{
public:
	PacketMgr();
	~PacketMgr();
	Packet* CreatePakcet(int msg_id, const char* msg, int msglen);
	void RegisterHandle(int msg_id, MsgHandle handle);
	void RegisterPacket(int msg_id, PPacket* pPacket);
	MsgHandle GetMsgHandle(int msg_id);
	void Clear();
private:
	std::unordered_map<int, MsgHandle> m_mHandle;
	std::unordered_map<int, PPacket*> m_mPackets;
};

extern std::unique_ptr<PacketMgr> g_pPacketMgr;
