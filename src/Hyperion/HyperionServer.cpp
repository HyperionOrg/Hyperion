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

		m_PacketManager = CreateScope<PacketManager>(m_Connections, std::bind(&HyperionServer::OnClientDisconnect, this, std::placeholders::_1));
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

		while (m_Running)
		{
			Update();
		}
	}

	void HyperionServer::Update()
	{
		while (!m_PacketsQueue.empty())
		{
			auto packet = m_PacketsQueue.pop_front();

			m_PacketManager->ProcessPacket(packet.Remote, packet.Packet);
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
					HP_INFO("Connection {0} Approved! (Current alive Connections: {1})", m_Connections.back()->GetId(), m_Connections.size());
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
		HP_INFO("Client Connected");
		return true;
	}

	void HyperionServer::OnClientDisconnect(Ref<Connection> client)
	{
		HP_INFO("Client Disconnected");
	}
}
