#pragma once

#include "Core.h"
#include "Connection.h"

namespace Hyperion
{
	class Client : public std::enable_shared_from_this<Client>
	{
	public:
		enum class State
		{
			HANDSHAKE = 0,
			STATUS,
			LOGIN,
			PLAY,
			DISCONNECT
		};

	private:
		int32_t m_ProtocolVersion;
		State m_CurrentState;
		Ref<Connection> m_Connection;

		friend class HandshakePacketHandler;

	public:
		Client();

		void Init(asio::io_context& context, asio::ip::tcp::socket socket, ThreadSafeQueue<OwnedPacket>& packetsIn);

		void ConnectClient(size_t connectionId);
		bool ReadNextPackets();

		void SendPacket(Ref<Packet> packet);

		bool IsAlive() const;

		State GetCurrentState() const;
		size_t GetConnectionId() const;
	};
}