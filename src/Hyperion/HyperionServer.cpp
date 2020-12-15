#include "HyperionServer.h"

#include <iostream>

namespace Hyperion
{
	HyperionServer::HyperionServer()
		: m_Port(25565), m_Acceptor(m_Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), m_Port))
		// TODO: Adding Configuration / Properties
	{
		Init();
	}

	HyperionServer::~HyperionServer()
	{
		Shutdown();
	}

	void HyperionServer::Init()
	{
		Log::Init();
	}

	void HyperionServer::Start()
	{
		try
		{
			WaitForClients();

			m_ContextThread = std::thread([this]() { m_Context.run(); });
		}
		catch (const std::exception&)
		{
			HP_ASSERT(false, "Server couldn't start properly");
			return;
		}

		HP_INFO("Starting minecraft server version 1.16.4");
	}

	void HyperionServer::Shutdown()
	{
		m_Context.stop();

		if (m_ContextThread.joinable()) m_ContextThread.join();

		HP_INFO("Stopping the server");
	}

	void HyperionServer::Run()
	{
		Start();

		std::string line;
		while (m_Running)
		{
			while (std::getline(std::cin, line))
			{
				if (line.empty() || line == "")
					break;

				HP_WARN("The Command '{0}' was not found!", line);
			}
			}
		}
	}

	void HyperionServer::Update()
	{
		while (!m_PacketsQueue.empty())
		{
			auto packet = m_PacketsQueue.pop_front();

			OnPacket(packet.Remote, packet.Packet);
		}
	}

	void HyperionServer::WaitForClients()
	{
		m_Acceptor.async_accept([this](std::error_code errorCode, asio::ip::tcp::socket socket)
			{
				HP_ASSERT(!errorCode, "Connection Error: {0}", errorCode.message());

				HP_INFO("New Connection: {0}", socket.remote_endpoint().address().to_string());
				Ref<Connection> connection = CreateRef<Connection>(m_Context, std::move(socket), m_PacketsQueue);

				if (OnClientConnect(connection))
				{
					m_Connections.push_back(std::move(connection));
					m_Connections.back()->ConnectToClient(m_ConnectionCounter++);
					HP_INFO("Connection {0} Approved!", m_Connections.back()->GetId());
				}
				else
				{
					HP_INFO("Connection Denied!");
				}

				WaitForClients();
			});
	}

	bool HyperionServer::OnClientConnect(Ref<Connection> client)
	{
		return true;
	}

	void HyperionServer::OnClientDisconnect(Ref<Connection> client)
	{

	}

	void HyperionServer::OnPacket(Ref<Connection> client, const Ref<Packet>& packet)
	{

	}

	void HyperionServer::SendPacketToAllClients(const Ref<Packet>& packet)
	{
		bool invalidClient = false;
		for (auto& client : m_Connections)
		{
			if (client && client->IsConnected())
			{
				client->SendPacket(packet);
			}
			else
			{
				OnClientDisconnect(client);
				client.reset();
				invalidClient = true;
			}
		}

		if (invalidClient)
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), nullptr), m_Connections.end());
	}

	void HyperionServer::SendPacketToClient(Ref<Connection> client, const Ref<Packet>& packet)
	{
		if (client && client->IsConnected())
		{
			client->SendPacket(packet);
		}
		else
		{
			OnClientDisconnect(client);
			client.reset();
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), client), m_Connections.end());
		}
	}

	void HyperionServer::SendPacketToClients(std::vector<Ref<Connection>>& clients, const Ref<Packet>& packet)
	{

	}
}
