#pragma once

#include <vector>

#include "Core.h"
#include "Utilities/VarInt.h"

namespace Hyperion
{
	class Packet
	{
	private:
		int32_t m_Id;
		int32_t m_Length;
		std::vector<uint8_t> m_Data;

		friend class Connection;
		friend class PacketHandler;

	public:
		Packet() = default;
		~Packet() = default;

		int32_t GetId() const { return m_Id; }

	protected:
		virtual void Serialize() {}
		virtual void Deserialize() {}

		void WriteVarInt(int32_t value)
		{
			std::vector<uint8_t> bytes = VarInt::Encode(value);
			for (uint8_t byte : bytes)
				m_Data.push_back(byte);
		}

		void WriteString(const std::string& value)
		{
			std::vector<uint8_t> bytes = VarInt::Encode(static_cast<int32_t>(value.size()));
			for (uint8_t byte : bytes)
				m_Data.push_back(byte);

			for (size_t i = 0; i < value.size(); i++)
				m_Data.push_back(static_cast<uint8_t>(value[i]));
		}

		void WriteInt64(int64_t value)
		{
			for (size_t i = 0; i < sizeof(int64_t); i++)
				m_Data.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
		}

		void WriteInt32(int32_t value)
		{
			for (size_t i = 0; i < sizeof(int16_t); i++)
				m_Data.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
		}

		void WriteInt16(int16_t value)
		{
			for (size_t i = 0; i < sizeof(int8_t); i++)
				m_Data.push_back(static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
		}

		void WriteInt8(int8_t value)
		{
			m_Data.push_back(static_cast<uint8_t>(value & 0xFF));
		}

		int32_t ReadVarInt()
		{
			return VarInt::Decode(m_Data);
		}

		std::string ReadString()
		{
			auto length = static_cast<size_t>(ReadVarInt());

			std::string string;

			for (size_t i = 0; i < length; i++)
				string += static_cast<char>(m_Data.at(i));

			m_Data.erase(m_Data.begin(), m_Data.begin() + length);

			return string;
		}

		int64_t ReadInt64()
		{
			int64_t value = 0;

			for (size_t i = 0; i < sizeof(int64_t); i++)
				value |= (m_Data.at(i) << ((sizeof(int64_t) - i) * 8));

			m_Data.erase(m_Data.begin(), m_Data.begin() + sizeof(int64_t));
			return value;
		}

		int32_t ReadInt32()
		{
			int32_t value = 0;

			for (size_t i = 0; i < sizeof(int32_t); i++)
				value |= (m_Data.at(i) << ((sizeof(int32_t) - i) * 8));

			m_Data.erase(m_Data.begin(), m_Data.begin() + sizeof(int32_t));
			return value;
		}

		int16_t ReadInt16()
		{
			int16_t value = 0;

			for (size_t i = 0; i < sizeof(int16_t); i++)
				value |= (m_Data.at(i) << ((sizeof(int16_t) - i) * 8));

			m_Data.erase(m_Data.begin(), m_Data.begin() + sizeof(int16_t));
			return value;
		}

		int8_t ReadInt8()
		{
			int8_t value = m_Data.at(0);
			m_Data.erase(m_Data.begin(), m_Data.begin() + 1);
			return value;
		}

	private:
		void Encode()
		{
			Serialize();
			std::vector<uint8_t> data = VarInt::Encode(static_cast<uint32_t>(m_Data.size()));
			for (std::vector<uint8_t>::reverse_iterator it = data.rbegin(); it != data.rend(); ++it)
				m_Data.insert(m_Data.begin(), *it);
		}

		void Decode()
		{
			Deserialize();
		}

		friend class PacketHandler;
	};

	class Client;

	struct OwnedPacket
	{
		Ref<Client> Remote;
		Ref<Packet> Packet;
	};
}