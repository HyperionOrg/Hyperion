#include "HyperionServer.h"

#include <iostream>

namespace Hyperion
{
	HyperionServer::HyperionServer()
		: m_Port(25565), m_Acceptor(m_Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), m_Port))
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

		// TODO: Add config

		m_PacketManager = CreateScope<PacketManager>(m_Clients, std::bind(&HyperionServer::OnClientDisconnect, this, std::placeholders::_1));
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
			for (auto& client : m_Clients)
			{
				if (client->ReadNextPackets())
					continue;
				OnClientDisconnect(client);
				client.reset();
				m_Clients.erase(std::remove(m_Clients.begin(), m_Clients.end(), client), m_Clients.end());
			}

			while (!m_PacketsQueue.empty())
			{
				auto packet = m_PacketsQueue.pop_front();
				m_PacketManager->ProcessPacket(packet.Remote, packet.Packet);
			}

			// TODO: Implement async commands
		}
	}

	void HyperionServer::WaitForClients()
	{
		m_Acceptor.async_accept([this](std::error_code connectionError, asio::ip::tcp::socket socket)
			{
				if (connectionError)
				{
					HP_ERROR("Connection Error: {0}", connectionError.message());
					HP_ASSERT(false, "Connecting failed!");
				}
				else
				{
					Ref<Client> client = CreateRef<Client>();
					client->Init(m_Context, std::move(socket), m_PacketsQueue);

					OnClientConnect(client);

					m_Clients.push_back(std::move(client));
					m_Clients.back()->ConnectClient(m_ConnectionCounter++);

					WaitForClients();
				}
			});
	}

	void HyperionServer::OnClientConnect(Ref<Client> client)
	{
	}

	void HyperionServer::OnClientDisconnect(Ref<Client> client)
	{
	}
}
