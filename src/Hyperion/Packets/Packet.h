#pragma once

#include <vector>

#include "Core.h"
#include "Utilities/VarInt.h"

namespace Hyperion
{
	class Packet
	{
	protected:
		VarInt m_Id;
		VarInt m_Length;
		std::vector<uint8_t> m_Data;

	public:
		Packet()
			: m_Id(0), m_Length(0), m_Data({}) {}

		Packet(const Ref<Packet> packet)
			: m_Id(packet->GetId()), m_Length(packet->GetLength()), m_Data(packet->GetData())
		{
		}

		VarInt GetId() const { return m_Id; }
		VarInt GetLength() const { return m_Length; }

		std::vector<uint8_t>& GetData() { return m_Data; }
		const std::vector<uint8_t>& GetData() const { return m_Data; }

	protected:
		void Encode()
		{
			Serialize();
		}

		void Decode()
		{
			m_Length = VarInt(m_Data[0]);
			m_Id = VarInt(m_Data[1]);
			m_Data.erase(m_Data.begin(), m_Data.begin() + 2);
			Deserialize();
		}

		virtual void Serialize() {}
		virtual void Deserialize() {}
	};

	class Connection;

	struct OwnedPacket
	{
		Ref<Connection> Remote;
		Ref<Packet> Packet;
	};
}