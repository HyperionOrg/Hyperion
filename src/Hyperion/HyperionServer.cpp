#include "Hyperion/HyperionServer.h"

#include <iostream>
#include <fstream>

#include "Hyperion/HyperUtilities/Random.h"

namespace Hyperion
{
	HyperionServer::HyperionServer()
		: m_Properties{ "server.properties" }
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

		HP_INFO("Starting minecraft server version 1.16.5");
		m_Timer.Start();

		HP_INFO("Loading properties...");
		m_Properties.SetProperty("spawn-protection", 16);
		m_Properties.SetProperty("gamemode", static_cast<std::string>("creative"));
		m_Properties.SetProperty("pvp", true);
		m_Properties.SetProperty("hardcore", false);
		m_Properties.SetProperty("max-players", 1);
		m_Properties.SetProperty("server-port", 25565);
		m_Properties.SetProperty("white-list", false);
		m_Properties.SetProperty("online-mode", true);
		m_Properties.SetProperty("motd", static_cast<std::string>(""));
		m_Properties.Load();
		m_Properties.Store();

		m_PacketManager = CreateScope<PacketManager>(m_Properties, m_Clients);

		std::optional<uint16_t> port = m_Properties.GetInt("server-port");
		m_Acceptor = CreateScope<asio::ip::tcp::acceptor>(m_Context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port.has_value() ? port.value() : 25565));
	
		Start();
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

		m_CommandThread = std::thread([this]()
			{
				std::string line;
				while (std::getline(std::cin, line))
				{
					if (line == "stop")
					{
						Shutdown();
						m_Running = false;
						break;
					}
				}
			});
	}

	void HyperionServer::Shutdown()
	{
		m_Context.stop();
		if (m_ContextThread.joinable()) m_ContextThread.join();

		HP_INFO("Stopping the server");
	}

	void HyperionServer::Run()
	{
		while (m_Running)
		{
			if (!m_Clients.empty())
			{
				for (Ref<Client>& client : m_Clients)
				{
					if (client && client->ReadNextPackets())
						continue;
					client.reset();
					m_Clients.erase(std::remove(m_Clients.begin(), m_Clients.end(), client), m_Clients.end());
				}
			}

			while (!m_PacketsQueue.empty())
			{
				auto packet = m_PacketsQueue.pop_front();
				m_PacketManager->ProcessPacket(packet.Remote, packet.ClientPacket);
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
					HP_ERROR("Client Connection Error: {0}", connectionError.message());
					return;
				}

				Ref<Client> client = CreateRef<Client>();
				client->Init(m_Context, std::move(socket), m_PacketsQueue);
				HP_INFO("Client Connected");

				m_Clients.push_back(std::move(client));
				m_Clients.back()->ConnectClient(m_ConnectionCounter++);

				WaitForClients();

			});
	}
}
