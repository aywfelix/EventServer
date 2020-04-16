#include "ChatNodeServer.h"
#include "JsonConfig.h"
#include "LogUtil.h"
#include "SeFNodeNet.pb.h"
#include "SeFNet.h"


bool ChatNodeServer::InitHelper()
{
	mNetServModule->AddReceiveCallBack(GAME_ROUTE_TO_CHAT, this, &ChatNodeServer::OnGameRouteChat);

	//init server info
	if (!mNetServModule->InitNet(g_pConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init ChatNodeServer failed");
		return false;
	}
	CLOG_INFO << "init ChatNodeServer ok" << CLOG_END;
	return true;
}


bool ChatNodeServer::SendToGame(int server_id, const int msg_id, google::protobuf::Message* msg)
{

	return true;
}

bool ChatNodeServer::SendToGame(const int msg_id, google::protobuf::Message* msg)
{
	std::string send_msg = msg->SerializeAsString();
	ChatToGamePacket game_packet;
	game_packet.set_msg_id(msg_id);
	game_packet.set_msg_body(send_msg);
	game_packet.set_player_id(0);
	SendPbByServType(ServerType::SERVER_TYPE_GAME, CHAT_ROUTE_TO_GAME, &game_packet);
	return true;
}

void ChatNodeServer::OnGameRouteChat(socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
}

