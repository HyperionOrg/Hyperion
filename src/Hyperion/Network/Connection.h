#pragma once

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif
#endif

#include <asio.hpp>

#include "Core.h"
#include "Packets/Packet.h"
#include "Utilities/ThreadSafeQueue.h"

namespace Hyperion
{
	class Connection : public std::enable_shared_from_this<Connection>
	{
	private:
		asio::io_context& m_Context;
		asio::ip::tcp::socket m_Socket;

		size_t m_Id;

		ThreadSafeQueue<Ref<Packet>> m_PacketsOut;
		ThreadSafeQueue<OwnedPacket>& m_PacketsIn;

		Ref<Packet> m_TempPacket;

	public:
		Connection(asio::io_context& context, asio::ip::tcp::socket socket, ThreadSafeQueue<OwnedPacket>& packetsIn);
		~Connection();
		
		void ConnectToClient(size_t id = 0);
		void Disconnect();

		void SendPacket(const Ref<Packet>& packet);
	
		bool IsConnected() const;

		size_t GetId() const { return m_Id; }

		void ReadPacket();

	private:
		void WriteBody();
	};
}