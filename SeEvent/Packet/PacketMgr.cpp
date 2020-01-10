
#include "Packet/PacketMgr.h"

std::unique_ptr<PacketMgr> g_pPacketMgr = nullptr;

PacketMgr::PacketMgr()
{}
PacketMgr::~PacketMgr()
{
	for (auto it : m_mPackets)
	{
		delete it.second;
	}
	Clear();
}
Packet* PacketMgr::CreatePakcet(int msg_id, const char* msg, int msglen)
{
	auto it = m_mPackets.find(msg_id);
	if (it == m_mPackets.end())
	{
		return nullptr;
	}
	return it->second->CreatePacket(msg_id, msg, msglen);
}

void PacketMgr::RegisterHandle(int msg_id, MsgHandle handle)
{
	auto it = m_mHandle.find(msg_id);
	if (it == m_mHandle.end())
	{
		m_mHandle.emplace(msg_id, handle);
	}
}

void PacketMgr::RegisterPacket(int msg_id, PPacket* pPacket)
{
	auto it = m_mPackets.find(msg_id);
	if (it == m_mPackets.end())
	{
		m_mPackets.emplace(msg_id, pPacket);
	}
}

MsgHandle PacketMgr::GetMsgHandle(int msg_id)
{
	auto it = m_mHandle.find(msg_id);
	if (it != m_mHandle.end())
	{
		return it->second;
	}
	return nullptr;
}

void PacketMgr::Clear()
{
	m_mHandle.clear();
	m_mPackets.clear();
}
