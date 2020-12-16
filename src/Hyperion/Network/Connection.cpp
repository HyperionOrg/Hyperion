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
	}

	void Connection::ConnectToClient(size_t id)
	{
		if (IsConnected())
		{
			m_Id = id;
			ReadPacket();
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
					WriteBody();
			});
	}

	bool Connection::IsConnected() const
	{
		return m_Socket.is_open();
	}

	void Connection::WriteBody()
	{
		asio::async_write(m_Socket, asio::buffer(m_PacketsOut.front()->GetData()), [this](std::error_code errorCode, size_t length)
			{
				if (errorCode)
				{
					HP_ASSERT(false, "Connection {1} - Write Body failed!", m_Id);
					m_Socket.close();
				}
				else
				{
					HP_INFO("Write Body successfully!");
					m_PacketsOut.pop_front();

					if (!m_PacketsOut.empty())
						WriteBody();
				}
			});
	}

	void Connection::ReadPacket()
	{
		m_TempPacket->GetData().resize(1);
		asio::async_read(m_Socket, asio::buffer(m_TempPacket->GetData()), [this](std::error_code sizeError, size_t sizeReadSize)
			{
				if (m_TempPacket->GetData()[0] < 250)
					if (sizeError)
					{
						HP_ERROR("Size reading error: {0}", sizeError.message());
						HP_ASSERT(sizeError == asio::error::eof, "Read Size failed!", m_Id);
						m_Socket.close();
					}
					else
					{
						int32_t dataSize = VarInt::Decode(m_TempPacket->GetData()[0]);
						HP_INFO("Read Size successfully!");
						m_TempPacket->GetData().resize(dataSize);

						asio::async_read(m_Socket, asio::buffer(m_TempPacket->GetData()), [this, dataSize](std::error_code bodyError, size_t dataReadSize)
							{
								if (bodyError)
								{
									HP_ERROR("Body reading error: {0}", bodyError.message());
									HP_ASSERT(bodyError == asio::error::eof, "Read Body failed!", m_Id);
									m_Socket.close();
								}
								else
								{
									HP_INFO("Read Body successfully!");
									m_TempPacket->GetData().resize(dataReadSize);

									m_TempPacket->m_Length = dataSize;
									m_TempPacket->m_Id = VarInt::Decode(m_TempPacket->GetData()[0]);
									m_TempPacket->GetData().erase(m_TempPacket->GetData().begin());

									m_PacketsIn.push_back({ this->shared_from_this(), m_TempPacket });
								}
							});
					}
			});
	}
}