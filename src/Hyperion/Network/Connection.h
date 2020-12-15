#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#include <asio.hpp>

#include "Core.h"
#include "Packet.h"
#include "Utilities/ThreadSafeQueue.h"

namespace Hyperion
{
	class Connection : public std::enable_shared_from_this<Connection>
	{
	private:
		asio::io_context& m_Context;
		asio::ip::tcp::socket m_Socket;

		uint32_t m_Id;

		ThreadSafeQueue<Ref<Packet>> m_PacketsOut;
		ThreadSafeQueue<OwnedPacket>& m_PacketsIn;

		Ref<Packet> m_TempPacket;

	public:
		Connection(asio::io_context& context, asio::ip::tcp::socket socket, ThreadSafeQueue<OwnedPacket>& packetsIn);
		~Connection();
		
		void ConnectToClient(uint32_t id = 0);
		void Disconnect();

		void SendPacket(const Ref<Packet>& packet);
	
		bool IsConnected() const;

		uint32_t GetId() const { return m_Id; }

	private:
		void WriteBody();
		void ReadBody();
	};
}