#pragma once

#include <asio.hpp>

#include <string>
#include <thread>
#include <vector>

#include "Hyperion/Core.h"
#include "Hyperion/HyperNetwork/Client.h"
#include "Hyperion/HyperNetwork/Connection.h"
#include "Hyperion/HyperNetwork/Packet.h"
#include "Hyperion/HyperNetwork/PacketManager.h"
#include "Hyperion/HyperUtilities/Properties.h"
#include "Hyperion/HyperUtilities/ThreadSafeQueue.h"
#include "Hyperion/HyperUtilities/Timer.h"

namespace Hyperion
{
	class HyperionServer
	{
	private:
		Properties m_Properties;
		Timer m_Timer;

		Scope<PacketManager> m_PacketManager;
		std::thread m_CommandThread;

		asio::io_context m_Context;
		std::thread m_ContextThread;

		Scope<asio::ip::tcp::acceptor> m_Acceptor;
		std::vector<Ref<Client>> m_Clients;
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
	};
}
