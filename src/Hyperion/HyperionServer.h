#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#include <asio.hpp>

#include <string>
#include <thread>
#include <vector>

#include "Core.h"
#include "Network/Connection.h"
#include "Network/Packet.h"
#include "Utilities/ThreadSafeQueue.h"

namespace Hyperion
{
	class HyperionServer
	{
	private:
		/* Data */
		uint16_t m_Port;

		bool m_Running = true;

		/* Networking */
		asio::io_context m_Context;
		std::thread m_ContextThread;

		asio::ip::tcp::acceptor m_Acceptor;
		std::deque<Ref<Connection>> m_Connections;

		ThreadSafeQueue<OwnedPacket> m_PacketsQueue;

		size_t m_ConnectionCounter = 0;

	public:
		HyperionServer();
		~HyperionServer();

		void Start();
		void Run();
		void Update();

		void SendPacketToAllClients(const Ref<Packet>& packet);
		void SendPacketToClient(Ref<Connection> client, const Ref<Packet>& packet);
		void SendPacketToClients(std::vector<Ref<Connection>>& clients, const Ref<Packet>& packet);

	private:
		void Init();
		void Shutdown();

		void WaitForClients();

		bool OnClientConnect(Ref<Connection> client);
		void OnClientDisconnect(Ref<Connection> client);
		void OnPacket(Ref<Connection> client, const Ref<Packet>& packet);
	};
}