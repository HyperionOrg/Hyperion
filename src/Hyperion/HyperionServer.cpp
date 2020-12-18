#include "HyperionServer.h"

#include <iostream>
#include <fstream>

#include "Utilities/Random.h"

namespace Hyperion
{
	HyperionServer::HyperionServer()
	{
		Init();
	}

	HyperionServer::~HyperionServer()
	{
		Shutdown();
	}

	void HyperionServer::Init()
	{
		m_Timer.Restart();
	
		Log::Init();
		Random::Init();
		HP_INFO("Starting minecraft server version 1.16.4");

		HP_INFO("Loading properties");
		m_Properties = Properties("server.properties");
		if (m_Properties.Exists())
		{
			m_Properties.Load();
		}
		else
		{
			m_Properties.SetProperty("spawn-protection", 16);
			m_Properties.SetProperty("gamemode", static_cast<std::string>("creative"));
			m_Properties.SetProperty("pvp", true);
			m_Properties.SetProperty("hardcore", false);
			m_Properties.SetProperty("max-players", 1);
			m_Properties.SetProperty("server-port", 25565);
			m_Properties.SetProperty("white-list", false);
			m_Properties.SetProperty("online-mode", true);
			m_Properties.SetProperty("motd", static_cast<std::string>(""));
			m_Properties.Store();
		}

		m_PacketManager = CreateScope<PacketManager>(m_Properties, m_Clients, std::bind(&HyperionServer::OnClientDisconnect, this, std::placeholders::_1));

		std::optional<uint16_t> port = m_Properties.GetInt("server-port");
		m_Acceptor = CreateScope<asio::ip::tcp::acceptor>(m_Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port.has_value() ? port.value() : 25565));
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

		m_Timer.Stop();
		HP_INFO("Done ({0}s)! For help, type \"help\"", m_Timer.Elapsed() / 1000.0);
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
		m_Acceptor->async_accept([this](std::error_code connectionError, asio::ip::tcp::socket socket)
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
