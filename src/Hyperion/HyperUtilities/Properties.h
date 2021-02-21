#pragma once

#include <string>
#include <optional>
#include <variant>
#include <unordered_map>

namespace Hyperion
{
	class Properties
	{
	private:
		using PropertiesType = std::variant<bool, int32_t, std::string>;

		std::unordered_map<std::string, PropertiesType> m_Data;
		std::string m_FilePath;

	public:
		Properties(const std::string& filePath);

		void Store();
		void Load();

		void SetProperty(const std::string& key, PropertiesType value);

		std::optional<bool> GetBool(const std::string& key);
		std::optional<int32_t> GetInt(const std::string& key);
		std::optional<std::string> GetString(const std::string& key);
	};
}
