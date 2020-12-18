#pragma once

#include <functional>

#include "Core.h"
#include "Network/Client.h"
#include "Network/Packet.h"

namespace Hyperion
{
	class PacketHandler
	{
	private:
		std::function<void(Ref<Client>)> m_DisconnectFunction;
		std::vector<Ref<Client>>* m_Clients;

	public:
		virtual void ProcessPacket(Ref<Client> client, const Ref<Packet> packet) = 0;

	protected:
		void SendPacket(Ref<Client> client, const Ref<Packet> packet)
		{
			if (client->IsAlive())
			{
				packet->Encode();
				client->SendPacket(packet);
			}
			else
			{
				m_DisconnectFunction(client);
				client.reset();
				m_Clients->erase(std::remove(m_Clients->begin(), m_Clients->end(), client), m_Clients->end());
			}
		}

		template <typename T, typename = typename std::enable_if<std::is_base_of<Packet, T>::value>>
		Ref<T> ToPacket(const Ref<Packet> packet)
		{
			Ref<T> p = CreateRef<T>();
			p->m_Data = packet->m_Data;

			p->Decode();

			return p;
		}
	};
}