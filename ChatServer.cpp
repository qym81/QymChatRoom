#include "ChatServer.h"

ChatServer::ChatServer()
{
	this->m_server.init_asio();
	// New Connect
	this->m_server.set_open_handler(
		[this](ConnectionHdl hdl)
		{
			std::lock_guard<std::mutex> lock(this->m_mutex);
			this->m_clients[hdl].name = "";	// Empty Name
			this->m_clients[hdl].onlineTime = std::time(nullptr);
			std::cout << "新连接，当前客户端数量： " << this->m_clients.size() << "\n";
		}
	);
	// Close
	this->m_server.set_close_handler(
		[this](ConnectionHdl hdl)
		{
			std::string name = "";
			{
				std::lock_guard<std::mutex> lock(this->m_mutex);
				auto it = this->m_clients.find(hdl);

				if (it != this->m_clients.end())
				{
					name = it->second.name;
					this->m_clients.erase(hdl);
				}
			}
			if (!name.empty())
			{
				this->Broadcast("[G]【系统】" + name + "已下线！");
				std::cout << name << "已下线！" << "\n";
			}
		}
	);
	// handle all the messages
	this->m_server.set_message_handler(
		[this](ConnectionHdl hdl, MessagePtr msg)
		{
			OnMessages(hdl, msg);
		}
	);
	
	// Close all the log
	this->m_server.clear_access_channels(websocketpp::log::alevel::all);
	this->m_server.clear_error_channels(websocketpp::log::elevel::all);
	
}

void ChatServer::Run(std::uint16_t port)
{
	this->m_server.listen(port);
	this->m_server.start_accept();
	std::cout << "监听 " << port << " 成功" << "\n";
	this->m_server.run();
}

void ChatServer::OnMessages(ConnectionHdl hdl, MessagePtr msg)
{
	const std::string text = msg->get_payload();

	std::string name = "";
	std::string error = "";
	bool signed_up = false;

	if (!GetName(hdl, name)) return;

	if (name.empty())
	{
		// Try to sign up
		if (text.empty())
		{
			error = "[R]【错误】昵称不能为空";
		}
		else
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			// Check same name
			for (const auto& client : this->m_clients)
			{
				if (text == client.second.name)
				{
					error = "[R]【错误】该昵称已被占用：" + text + "请重新输入名字！";
					break;
				}
			}
			// No error : sign up successfully
			// ::apply name and array
			if (error.empty())
			{
				name = text;
				this->m_clients[hdl].name = name;
				signed_up = true;
			}
		}
	}
	
	// Send error and return
	if (!error.empty())
	{
		SendTo(hdl, error);
		return;
	}
	// signed up successfully: broad cast
	if (signed_up)
	{
		std::cout << "登录成功：" << name << "\n";
		Broadcast("[G]【系统】" + name + " 已上线!");
		SendTo(hdl, "[G]【系统】欢迎" + name + "~" + GetOnlineList());
		SendTo(hdl, "[G]【系统】输入GetCmd或Command查看可用指令");
		return;
	}

	handleCommand(hdl, text);
}

void ChatServer::SendTo(ConnectionHdl hdl, const std::string& text)
{
	websocketpp::lib::error_code ec;
	this->m_server.send(hdl, text, websocketpp::frame::opcode::text, ec);

	if (ec)
	{
		std::cerr << "发送失败：" << ec.message() << "\n";
	}
}

void ChatServer::Broadcast(const std::string& text)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);
	for (auto& cl : this->m_clients)
	{
		if (!cl.second.name.empty())
		{
			// Send if its name isn't empty
			SendTo(cl.first, text);
		}
	}
}

bool ChatServer::GetName(ConnectionHdl hdl, std::string& name)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);
	auto it = this->m_clients.find(hdl);

	if (it != this->m_clients.end())
	{
		name = it->second.name;
		return true;
	}
	return false;
}

std::string ChatServer::GetCmd()
{
	std::string CmdStr;
	CmdStr += "[G]【功能提示】可用指令:\n";
	/*CmdStr += "[G]GetCmd / command		查询可用指令\n";
	CmdStr += "[G]GetWho				查询房间人员\n";
	CmdStr += "[G]GetMe				查看个人面板\n";
	CmdStr += "[G]tell					私聊格式:tell [用户名字] [私聊消息]\n";
	CmdStr += "[G]cls / clear			清屏\n";
	CmdStr += "[G]exit / quit			退出聊天室\n";*/
	CmdStr += "GetCmd / Command		查询可用指令\n";
	CmdStr += "GetWho				查询房间人员\n";
	CmdStr += "GetMe				查看个人面板\n";
	CmdStr += "tell					私聊格式:tell [用户名字] [私聊消息]\n";
	CmdStr += "cls / clear			清屏\n";
	CmdStr += "exit / quit			退出聊天室\n";
	return CmdStr;
}

std::string ChatServer::GetMe(ConnectionHdl hdl)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);
	auto it = this->m_clients.find(hdl);
	if (it == this->m_clients.end()) return "";

	std::time_t now = std::time(nullptr);
	std::time_t diff = now - it->second.onlineTime;

	int hour = static_cast<int>(diff / 3600);
	int min = static_cast<int>((diff % 3600) / 60);
	int seconds = static_cast<int>(diff % 60);

	char buf[128] = { 0 };
	std::snprintf(buf, sizeof(buf),
		"[G]-------\n【个人面板】\n昵称：%s\n在线时长：%02d:%02d:%02d (时:分:秒)\n-----------------",
		it->second.name.c_str(), hour, min, seconds);
	return std::string(buf);
}

std::string ChatServer::GetOnlineList()
{
	std::lock_guard<std::mutex> lock(this->m_mutex);
	std::string list = "";
	int count = 0;
	for (auto& cli : this->m_clients)
	{
		if (cli.second.name.empty()) continue;
		if (count++) list += "、";
		list += cli.second.name;
	}
	return "当前共" + std::to_string(count) + "人：" + list;
}

void ChatServer::handleCommand(ConnectionHdl hdl, const std::string& text)
{
	std::string name = "";
	if(!GetName(hdl, name)) return;

	if (text == "cls" || text == "clear")
	{
		SendTo(hdl, "DO_CLS");
		std::cout << name << "执行了清屏！" << "\n";
		return;
	}
	if (text == "GetCmd" || text == "Command")
	{
		SendTo(hdl, GetCmd());
		std::cout << name << "查询了可用指令！" << "\n";
		return;
	}
	if (text == "GetWho")
	{
		SendTo(hdl, "【查询】" + GetOnlineList());
		std::cout << name << "查询了在线人员！" << "\n";
		return;
	}
	if (text == "GetMe")
	{
		SendTo(hdl, GetMe(hdl));
		std::cout << name << "查看了个人面板！" << "\n";
		return;
	}
	if (text.rfind("tell ", 0) == 0)
	{
		handleTell(hdl, name, text);
		return;
	}
	if (text == "exit" || text == "quit")
	{
		this->m_server.close(hdl, 
			websocketpp::close::status::normal, "user exit");
		return;
	}

	std::cout << name << "：" << text << "\n";
	Broadcast("[" + name + "]：" + text);
}

void ChatServer::handleTell(ConnectionHdl hdl, const std::string& from, const std::string& text)
{
	size_t nickStart = text.find_first_not_of(" ", 5);
	if (nickStart == std::string::npos)
	{
		SendTo(hdl, "[R]【错误】私聊格式：tell [用户名字] [私聊消息]");
		return;
	}

	size_t contentStart = text.find(" ", nickStart);
	if (contentStart == std::string::npos)
	{
		SendTo(hdl, "[R]【错误】请输入私聊内容");
		return;
	}

	std::string target = text.substr(nickStart, contentStart - nickStart);
	std::string content = text.substr(text.find_first_not_of(' ', contentStart));

	// find target
	ConnectionHdl targetHdl;
	bool found = false;
	{
		std::lock_guard<std::mutex> lock(this->m_mutex);
		for (auto& cli : this->m_clients)
		{
			if (target == cli.second.name)
			{
				targetHdl = cli.first;
				found = true;
				break;
			}
		}
	}

	if (!found)
	{
		SendTo(hdl, "[R]【错误】私聊：用户" + target + "不存在或未在线");
		return;
	}

	SendTo(targetHdl, "【私聊】" + from + "->你：" + content);
	SendTo(hdl, "【私聊】你->" + target + "：" + content);
	std::cout << from << "向" << target << "发送了一条私聊消息~" << "\n";
}
