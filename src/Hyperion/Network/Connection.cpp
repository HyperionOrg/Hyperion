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

	void Connection::ConnectToClient(uint32_t id)
	{
		if (IsConnected())
		{
			m_Id = id;
			ReadBody();
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
		asio::async_write(m_Socket, asio::buffer((void*)m_PacketsOut.front()->GetData().data(), m_PacketsOut.front()->GetData().size()), [this](std::error_code errorCode, size_t length)
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

	void WriteVarInt(uint32_t i)
	{
		while (true)
		{
			if ((i & 0xFFFFFF80) == 0)
			{
				HP_WARN("{0}", i & 0x7F | 0x80);
				break;
			}

			HP_WARN("{0}", i & 0x7F | 0x80);
			i = i >> 7;
		}
	}

	void Connection::ReadBody()
	{
		m_TempPacket->GetData().resize(1);
		asio::async_read(m_Socket, asio::buffer(m_TempPacket->GetData()), [this](std::error_code sizeError, size_t sizeSize)
			{
				if (sizeError)
				{
					HP_ERROR("Size reading error: {0}", sizeError.message());
					HP_ASSERT(false, "Read Size failed!", m_Id);
					m_Socket.close();
				}
				else
				{
					HP_INFO("Read Size successfully!");
					HP_DEBUG("Size: {0}", VarInt::Decode(m_TempPacket->GetData()[0]));
					m_TempPacket->GetData().resize(VarInt::Decode(m_TempPacket->GetData()[0]));

					asio::async_read(m_Socket, asio::buffer(m_TempPacket->GetData()), [this](std::error_code bodyError, size_t dataSize)
						{
							if (bodyError && bodyError != asio::error::eof)
							{
								HP_ERROR("Body reading error: {0}", bodyError.message());
								HP_ASSERT(false, "Read Body failed!", m_Id);
								m_Socket.close();
							}
							else
							{
								HP_INFO("Read Body successfully!");
								m_TempPacket->GetData().resize(dataSize);
								m_PacketsIn.push_back({ this->shared_from_this(), m_TempPacket });
							}
						});
				}
			});
	}
}