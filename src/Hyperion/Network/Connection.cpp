#include "Connection.h"

namespace Hyperion
{
	Connection::Connection(asio::io_context& context, asio::ip::tcp::socket socket, ThreadSafeQueue<OwnedPacket>& packetsIn)
		: m_Context(context), m_Socket(std::move(socket)), m_PacketsIn(packetsIn)
	{
	}

	Connection::~Connection()
	{
	}

	void Connection::ConnectToClient(uint32_t id)
	{
		if (IsConnected())
			m_Id = id;
	}

	void Connection::Disconnect()
	{
		if (IsConnected())
			asio::post(m_Context, [this]() { m_Socket.close(); });
	}

	void Connection::SendPacket(const Ref<Packet>& packet)
	{
		asio::post(m_Context, [this, packet]()
			{
				bool writingPacket = m_PacketsOut.empty();

				m_PacketsOut.push_back(packet);

				if (writingPacket)
					WriteBody();
			});
	}

	bool Connection::IsConnected() const
	{
		return m_Socket.is_open();
	}

	void Connection::WriteBody()
	{
		asio::async_read(m_Socket, asio::buffer((void*)m_PacketsOut.front()->GetData().data(), m_PacketsOut.front()->GetData().size()), [this](std::error_code errorCode, size_t length)
			{
				if (errorCode)
				{
					HP_ASSERT(false, "Connection {0} - Write Body failed!", m_Id);
					m_Socket.close();
				}
				else
				{
					m_PacketsOut.pop_front();

					if (!m_PacketsOut.empty())
						WriteBody();
				}
			});
	}

	void Connection::ReadBody()
	{
		asio::async_read(m_Socket, asio::buffer((void*)m_TempPacket->GetData().data(), m_TempPacket->GetData().size()), [this](std::error_code errorCode, size_t length)
			{
				if (errorCode)
				{
					HP_ASSERT(false, "Connection {0} - Read Body failed!", m_Id);
					m_Socket.close();
				}
				else
				{
					m_PacketsIn.push_back({ this->shared_from_this(), m_TempPacket });
				}
			});
	}
}