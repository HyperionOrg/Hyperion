#pragma once

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#endif

#include <asio.hpp>

#include <string>
#include <thread>
#include <vector>

#include "Core.h"
#include "Network/Client.h"
#include "Network/Connection.h"
#include "Network/Packet.h"
#include "Network/PacketManager.h"
#include "Utilities/Properties.h"
#include "Utilities/ThreadSafeQueue.h"
#include "Utilities/Timer.h"

namespace Hyperion
{
	class HyperionServer
	{
	private:
		Properties m_Properties;
		Scope<PacketManager> m_PacketManager;

		Timer m_Timer;

		asio::io_context m_Context;
		std::thread m_ContextThread;

		Scope<asio::ip::tcp::acceptor> m_Acceptor;
		std::deque<Ref<Client>> m_Clients;
		size_t m_ConnectionCounter = 0;

		ThreadSafeQueue<OwnedPacket> m_PacketsQueue;

		bool m_Running = true;

	public:
		HyperionServer();
		~HyperionServer();

		void Start();
		void Run();

	private:
		void Init();
		void Shutdown();

		void WaitForClients();

		void OnClientConnect(Ref<Client> client);
		void OnClientDisconnect(Ref<Client> client);
	};
}