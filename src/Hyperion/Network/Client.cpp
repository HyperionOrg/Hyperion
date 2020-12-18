#include "Client.h"

namespace Hyperion
{
	Client::Client()
		: m_CurrentState(State::HANDSHAKE)
	{
	}

	void Client::Init(asio::io_context& context, asio::ip::tcp::socket socket, ThreadSafeQueue<OwnedPacket>& packetsIn)
	{
		m_Connection = CreateRef<Connection>(this->shared_from_this(), context, std::move(socket), packetsIn);
	}

	void Client::ConnectClient(size_t connectionId)
	{
		m_Connection->ConnectToClient(connectionId);
	}

	bool Client::ReadNextPackets()
	{
		if (!IsAlive())
			return false;
		m_Connection->ReadNextPackets();
		return true;
	}

	void Client::SendPacket(Ref<Packet> packet)
	{
		m_Connection->SendPacket(packet);
	}

	bool Client::IsAlive() const
	{
		return m_Connection && m_Connection->IsConnected();
	}

	Client::State Client::GetCurrentState() const
	{
		return m_CurrentState;
	}

	size_t Client::GetConnectionId() const
	{
		return m_Connection->GetId();
	}
}