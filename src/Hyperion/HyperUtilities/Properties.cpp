#include "Hyperion/HyperUtilities/Properties.h"

#include "Hyperion/Core.h"
#include "Hyperion/HyperUtilities/FileUtils.h"

namespace Hyperion
{
	Properties::Properties(const std::string& filePath)
		: m_FilePath(filePath)
	{
	}

	void Properties::Store()
	{
		HP_ASSERT(FileUtils::IsFile(m_FilePath), "Properties Path is not a file!");

		std::vector<std::string> lines;
		for (auto propertiesPair : m_Data)
		{
			if (auto value = std::get_if<bool>(&propertiesPair.second))
				lines.push_back(propertiesPair.first + "=" + (*value ? "true" : "false"));
			else if (auto value = std::get_if<int32_t>(&propertiesPair.second))
				lines.push_back(propertiesPair.first + "=" + std::to_string(*value));
			else if (auto value = std::get_if<std::string>(&propertiesPair.second))
				lines.push_back(propertiesPair.first + "=" + *value);
		}
		FileUtils::WriteFile(m_FilePath, lines);
	}

	void Properties::Load()
	{
		HP_ASSERT(FileUtils::IsFile(m_FilePath), "Properties Path is not a file!");

		if (FileUtils::Exists(m_FilePath))
		{
			FileUtils::ReadFile(m_FilePath, [this](const std::string& line)
				{
					std::string key = line.substr(0, line.find_first_of("="));
					std::string value = line.substr(line.find_first_of("=") + 1, line.length());

					if (value.empty())
					{
						m_Data[key] = value;
						return;
					}

					std::string::const_iterator it = value.begin();
					while (it != value.end() && std::isdigit(*it)) ++it;
					if (it == value.end())
						m_Data[key] = std::stoi(value);
					else if (value == "true" || value == "false")
						m_Data[key] = (value == "true");
					else
						m_Data[key] = value;
				});
		}
	}

	void Properties::SetProperty(const std::string& key, PropertiesType value)
	{
		m_Data[key] = value;
	}

	std::optional<bool> Properties::GetBool(const std::string& key)
	{
		if (m_Data.find(key) == m_Data.end())
			return std::nullopt;
		if (auto value = std::get_if<bool>(&m_Data.at(key)))
			return *value;
		return std::nullopt;
	}

	std::optional<int32_t> Properties::GetInt(const std::string& key)
	{
		if (m_Data.find(key) == m_Data.end())
			return std::nullopt;
		if (auto value = std::get_if<int32_t>(&m_Data.at(key)))
			return *value;
		return std::nullopt;
	}

	std::optional<std::string> Properties::GetString(const std::string& key)
	{
		if (m_Data.find(key) == m_Data.end())
			return std::nullopt;
		if (auto value = std::get_if<std::string>(&m_Data.at(key)))
			return *value;
		return std::nullopt;
	}
}
