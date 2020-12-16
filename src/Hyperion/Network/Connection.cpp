#include "Connection.h"

namespace Hyperion
{
	Connection::Connection(asio::io_context& context, asio::ip::tcp::socket socket, ThreadSafeQueue<OwnedPacket>& packetsIn)
		: m_Context(context), m_Socket(std::move(socket)), m_PacketsIn(packetsIn)
	{
		m_TempPacket = CreateRef<Packet>();
	}

	Connection::~Connection()
	{
		Disconnect();
	}

	void Connection::ConnectToClient(size_t id)
	{
		if (IsConnected())
		{
			m_Id = id;
			ReadPackets();
		}
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
					WritePacket();
			});
	}

	void Connection::ReadPackets()
	{
		if (IsConnected())
		{
			if (m_Socket.available() > 0 && !m_Reading)
				ReadPacket();
		}
	}

	bool Connection::IsConnected() const
	{
		return m_Socket.is_open();
	}

	void Connection::WritePacket()
	{
		asio::async_write(m_Socket, asio::buffer(m_PacketsOut.front()->GetData()), [this](std::error_code writeError, size_t length)
			{
				if (writeError)
				{
					HP_ERROR("Connection {0} - Packet writing error: {1}", m_Id, writeError.message());
					HP_ASSERT(false, "Write Packet failed!");
					m_Socket.close();
				}
				else
				{
					m_PacketsOut.pop_front();

					if (!m_PacketsOut.empty())
						WritePacket();
				}
			});
	}

	void Connection::ReadPacket()
	{
		m_Reading = true;
		asio::async_read(m_Socket, asio::buffer(m_TempPacket->GetData()), [this](std::error_code sizeError, size_t sizeReadSize)
			{
				if (m_TempPacket->GetData()[0] < 250)
					if (sizeError)
					{
						HP_ERROR("Connection {0} - Size reading error: {1}", m_Id, sizeError.message());
						HP_ASSERT(sizeError == asio::error::eof, "Read Size failed!");
						m_Socket.close();
					}
					else
					{
						int32_t dataSize = VarInt::Decode(m_TempPacket->GetData()[0]);
						m_TempPacket->GetData().resize(dataSize);

						asio::async_read(m_Socket, asio::buffer(m_TempPacket->GetData()), [this, dataSize](std::error_code bodyError, size_t dataReadSize)
							{
								if (bodyError)
								{
									HP_ERROR("Connection {0} - Body reading error: {0}", m_Id, bodyError.message());
									HP_ASSERT(bodyError == asio::error::eof, "Read Body failed!");
									m_Socket.close();
								}
								else
								{
									m_TempPacket->GetData().resize(dataReadSize);

									m_TempPacket->m_Length = dataSize;
									m_TempPacket->m_Id = VarInt::Decode(m_TempPacket->GetData()[0]);
									m_TempPacket->GetData().erase(m_TempPacket->GetData().begin());

									m_PacketsIn.push_back({ this->shared_from_this(), m_TempPacket });
									m_Reading = false;
								}
							});
					}
			});
	}
}