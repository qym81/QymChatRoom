#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <websocketpp/server.hpp>
#include <websocketpp/message_buffer/message.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <ctime>

using Server = websocketpp::server<websocketpp::config::asio>;
using MessagePtr = Server::message_ptr;
using ConnectionHdl = websocketpp::connection_hdl;
using HdlLess = std::owner_less<ConnectionHdl>;

struct ClientInfo
{
	std::string name;
	std::time_t onlineTime;
};

class ChatServer
{
public:
	ChatServer();
	void Run(std::uint16_t port);
private:
	// Functions
	void OnMessages(ConnectionHdl hdl, MessagePtr msg);
	void SendTo(ConnectionHdl hdl, const std::string& text);
	void Broadcast(const std::string& text);
	bool GetName(ConnectionHdl hdl, std::string& name);
	std::string GetCmd();
	std::string GetMe(ConnectionHdl hdl);
	std::string GetOnlineList();
	void handleCommand(ConnectionHdl hdl, const std::string& text);
	void handleTell(ConnectionHdl hdl, const std::string& from, const std::string& text);

	Server m_server;
	std::map<ConnectionHdl, ClientInfo, HdlLess> m_clients;
	std::mutex m_mutex;
};

#endif // !CHATSERVER_H


