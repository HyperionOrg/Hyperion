#include "PacketManager.hpp"

#include "PacketsIn/PacketInHandshake.h"
#include "PacketsIn/PacketInPing.h"

#include "PacketsOut/PacketOutPong.h"
#include "PacketsOut/PacketOutResponse.h"

namespace Hyperion
{
	PacketManager::PacketManager(std::deque<Ref<Connection>>& connections, std::function<void(Ref<Connection>)> disconnectFunction)
		: m_Connections(connections), m_DisconnectFunction(disconnectFunction)
	{
	}

	void PacketManager::ProcessPacket(Ref<Connection> client, const Ref<Packet>& packet)
	{
		if (!ProcessRequest(client, packet))
		{
			m_LastPacketId = static_cast<PacketInIds>(packet->GetId().GetData()[0]);
			switch (m_LastPacketId)
			{
			case Hyperion::PacketInIds::HANDSHAKE:
			{
				Ref<PacketInHandshake> handshakePacket = CreateRef<PacketInHandshake>(packet);
				ReadPacketFromClient(client); // Request Packet
				break;
			}
			case Hyperion::PacketInIds::PING:
			{
				Ref<PacketInPing> pingPacket = CreateRef<PacketInPing>(packet);
				Ref<PacketOutPong> pongPacket = CreateRef<PacketOutPong>(pingPacket->GetPayload());
				SendPacketToClient(client, pongPacket); // Pong Packet
				break;
			}
			default:
				break;
			}
		}
	}

	bool PacketManager::ProcessRequest(Ref<Connection> client, const Ref<Packet>& packet)
	{
		/* Request Packet */
		if (packet->GetLength().GetData()[0] != 1 || packet->GetId().GetData()[0] != 0x00)
			return false;

		switch (m_LastPacketId)
		{
		case Hyperion::PacketInIds::HANDSHAKE:

		{
			Ref<PacketOutResponse> responsePacket = CreateRef<PacketOutResponse>("{\"version\":{\"name\":\"1.16.4\",\"protocol\":754},\"players\":{\"max\":100,\"online\":5,\"sample\":[{\"name\":\"thinkofdeath\",\"id\":\"4566e69f-c907-48ee-8d71-d7ba5aa00d20\"}]},\"description\":{\"text\":\"Hello World\",\"color\":\"yellow\"}}");
			SendPacketToClient(client, responsePacket); // Response Packet
			ReadPacketFromClient(client); // Ping Packet
			break;
		}
		case Hyperion::PacketInIds::PING:
		{

			break;
		}
		default:
			break;
		}
		return true;
	}

	void PacketManager::SendPacketToAllClients(const Ref<Packet>& packet)
	{
		bool invalidClient = false;
		for (auto& client : m_Connections)
		{
			if (client && client->IsConnected())
			{
				client->SendPacket(packet);
				KillInvalidClient(client);
			}
			else
			{
				m_DisconnectFunction(client);
				client.reset();
				invalidClient = true;
			}
		}

		if (invalidClient)
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), nullptr), m_Connections.end());
	}

	void PacketManager::SendPacketToClient(Ref<Connection> client, const Ref<Packet>& packet)
	{
		if (client && client->IsConnected())
		{
			client->SendPacket(packet);
			KillInvalidClient(client);
		}
		else
		{
			m_DisconnectFunction(client);
			client.reset();
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), client), m_Connections.end());
		}
	}

	void PacketManager::SendPacketToClients(std::vector<Ref<Connection>>& clients, const Ref<Packet>& packet)
	{
		bool invalidClient = false;
		for (auto& client : clients)
		{
			if (client && client->IsConnected())
			{
				client->SendPacket(packet);
				KillInvalidClient(client);
			}
			else
			{
				m_DisconnectFunction(client);
				client.reset();
				invalidClient = true;
			}
		}

		if (invalidClient)
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), nullptr), m_Connections.end());
	}

	void PacketManager::ReadPacketFromAllClients()
	{
		bool invalidClient = false;
		for (auto& client : m_Connections)
		{
			if (client && client->IsConnected())
			{
				client->ReadPacket();
				KillInvalidClient(client);
			}
			else
			{
				m_DisconnectFunction(client);
				client.reset();
				invalidClient = true;
			}
		}

		if (invalidClient)
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), nullptr), m_Connections.end());
	}

	void PacketManager::ReadPacketFromClient(Ref<Connection> client)
	{
		if (client && client->IsConnected())
		{
			client->ReadPacket();
			KillInvalidClient(client);
		}
		else
		{
			m_DisconnectFunction(client);
			client.reset();
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), client), m_Connections.end());
		}
	}

	void PacketManager::ReadPacketFromClients(std::vector<Ref<Connection>>& clients)
	{
		bool invalidClient = false;
		for (auto& client : clients)
		{
			if (client && client->IsConnected())
			{
				client->ReadPacket();
				KillInvalidClient(client);
			}
			else
			{
				m_DisconnectFunction(client);
				client.reset();
				invalidClient = true;
			}
		}

		if (invalidClient)
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), nullptr), m_Connections.end());
	}

	void PacketManager::KillInvalidClient(Ref<Connection> client)
	{
		if (!client || !client->IsConnected())
		{
			m_DisconnectFunction(client);
			client.reset();
			m_Connections.erase(std::remove(m_Connections.begin(), m_Connections.end(), client), m_Connections.end());
		}
	}
}