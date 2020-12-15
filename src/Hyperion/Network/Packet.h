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
		Packet(VarInt id = 0)
			: m_Id(id), m_Length(sizeof(m_Id)), m_Data({}) {}

		virtual void Serialize() {}
		virtual void Deserialize() {}

		void CalculateLength() { m_Length = static_cast<VarInt>(sizeof(m_Id) + sizeof(uint8_t) * m_Data.size()); }

		VarInt GetId() const { return m_Id; }
		VarInt GetLength() const { return m_Length; }
		std::vector<uint8_t>& GetData() { return m_Data; }
		const std::vector<uint8_t>& GetData() const { return m_Data; }
	};

	class Connection;

	struct OwnedPacket
	{
		Ref<Connection> Remote;
		Ref<Packet> Packet;
	};
}