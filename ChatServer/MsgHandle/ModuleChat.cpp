#include "clientmodule/ModuleChat.h"
#include "ChatPlayer.h"
#include "LogHelper.h"
#include "Assertx.h"

int ModuleChat::ChatReq(Player* player, Packet* packet)
{
	Assert(player && packet);
	auto pMsg = (Chat_ChatReq*)packet->msg;
	auto chat_player = (ChatPlayer*)player;
	const std::string msg = pMsg->chat_msg();

	CLOG_INFO << "recv msg from gate server " << msg << CLOG_END;
	
	Chat_ChatReply reply;
	reply.set_ret(0);
	reply.set_chat_msg(msg);

	//chat_player->SendToGame();
	return 0;
}