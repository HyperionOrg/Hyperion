#pragma once

#include <json.hpp>

#include <string>
#include <vector>

namespace Hyperion
{
	struct ServerListPingInfo
	{
		struct VersionInfo
		{
			std::string Name;
			int32_t ProtocolVersion;
		} Version;

		struct PlayersInfo
		{
			int32_t Max;
			int32_t Online;

			struct SampleInfo
			{
				std::string Name;
				std::string Id;
			};
			std::vector<SampleInfo> Sample;
		} Players;
	};

	void to_json(nlohmann::json& json, const ServerListPingInfo::VersionInfo& versionInfo)
	{
		json = nlohmann::json{ { "name", versionInfo.Name }, { "protocol", versionInfo.ProtocolVersion } };
	}

	void from_json(const nlohmann::json& json, ServerListPingInfo::VersionInfo& versionInfo)
	{
		json.at("name").get_to(versionInfo.Name);
		json.at("protocol").get_to(versionInfo.ProtocolVersion);
	}

	void to_json(nlohmann::json& json, const ServerListPingInfo::PlayersInfo::SampleInfo& sampleInfo)
	{
		json = nlohmann::json{ { "name", sampleInfo.Name }, { "id", sampleInfo.Id } };
	}

	void from_json(const nlohmann::json& json, ServerListPingInfo::PlayersInfo::SampleInfo& sampleInfo)
	{
		json.at("name").get_to(sampleInfo.Name);
		json.at("id").get_to(sampleInfo.Id);
	}

	void to_json(nlohmann::json& json, const ServerListPingInfo::PlayersInfo& playersInfo)
	{
		json = nlohmann::json{ { "max", playersInfo.Max }, { "online", playersInfo.Online }, { "sample", playersInfo.Sample } };
	}

	void from_json(const nlohmann::json& json, ServerListPingInfo::PlayersInfo& playersInfo)
	{
		json.at("max").get_to(playersInfo.Max);
		json.at("online").get_to(playersInfo.Online);
		json.at("sample").get_to(playersInfo.Sample);
	}

	void to_json(nlohmann::json& json, const ServerListPingInfo& serverListPingInfo)
	{
		json = nlohmann::json{ { "version", serverListPingInfo.Version}, { "players", serverListPingInfo.Players } };
	}

	void from_json(const nlohmann::json& json, ServerListPingInfo& serverListPingInfo)
	{
		json.at("version").get_to(serverListPingInfo.Version);
		json.at("players").get_to(serverListPingInfo.Players);
	}
}