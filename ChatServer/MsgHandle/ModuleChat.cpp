#include "ClientModule/ModuleChat.h"
#include "Packet/Player.h"
#include "LogHelper.h"
#include "Assertx.h"

int ModuleChat::ChatReq(Player* pPlayer, Packet* pPacket)
{
	Assert(pPlayer && pPacket);
	auto pMsg = (Chat_ChatReq*)pPacket->pMsg;
	const std::string msg = pMsg->chat_msg();

	CLOG_INFO << "recv msg from gate server " << msg << CLOG_END;
	
	Chat_ChatReply reply;
	reply.set_ret(0);
	reply.set_chat_msg(msg);

	pPlayer->SendMsg(pPacket->msg_id, &reply);
	return 0;
}