#include "Hyperion/HyperNetwork/Connection.h"

#include "Hyperion/HyperNetwork/Client.h"

namespace Hyperion
{
	Connection::Connection(Ref<Client> client, asio::io_context& context, asio::ip::tcp::socket socket, ThreadSafeQueue<OwnedPacket>& packetsIn)
		: m_Client(client), m_Context(context), m_Socket(std::move(socket)), m_PacketsIn(packetsIn)
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
			ReadNextPackets();
		}
	}

	void Connection::Disconnect()
	{
		if (IsConnected())
		{
			asio::post(m_Context, [this]()
				{
					m_Socket.close();
				});
		}
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

	void Connection::ReadNextPackets()
	{
		if (m_Socket.available() > 0 && !m_Reading)
		{
			ReadPacket();
		}
	}

	bool Connection::IsConnected() const
	{
		return m_Socket.is_open();
	}

	void Connection::WritePacket()
	{
		asio::async_write(m_Socket, asio::buffer(m_PacketsOut.front()->m_Data), [this](std::error_code writeError, size_t length)
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

		m_TempPacket->m_Data.resize(1);
		asio::async_read(m_Socket, asio::buffer(m_TempPacket->m_Data), [this](std::error_code sizeError, size_t sizeReadSize)
			{
				if (sizeError || m_TempPacket->m_Data.front() > 250)
				{
					HP_ERROR("Connection {0} - Size reading error: {1}", m_Id, sizeError.message());
					HP_ASSERT(sizeError == asio::error::eof, "Read Size failed!");
					m_Socket.close();
				}
				else
				{
					int32_t dataSize = VarInt::Decode(m_TempPacket->m_Data);
					m_TempPacket->m_Data.resize(dataSize);

					asio::async_read(m_Socket, asio::buffer(m_TempPacket->m_Data), [this, dataSize](std::error_code bodyError, size_t dataReadSize)
						{
							if (bodyError)
							{
								HP_ERROR("Connection {0} - Body reading error: {0}", m_Id, bodyError.message());
								HP_ASSERT(bodyError == asio::error::eof, "Read Body failed!");
								m_Socket.close();
							}
							else
							{
								m_TempPacket->m_Data.insert(m_TempPacket->m_Data.begin(), dataSize);

								m_TempPacket->m_Length = VarInt::Decode(m_TempPacket->m_Data);
								m_TempPacket->m_Id = VarInt::Decode(m_TempPacket->m_Data);

								m_PacketsIn.push_back({ m_Client, m_TempPacket });
								m_Reading = false;
							}
						});
				}
			});
	}
}