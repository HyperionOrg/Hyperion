#include "Hyperion/HyperUtilities/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include "Hyperion/Core.h"

namespace Hyperion
{
	Ref<spdlog::logger> Log::s_Logger;

	void UppercaseFormatter::format(const spdlog::details::log_msg& message, const std::tm& time, spdlog::memory_buf_t& destination)
	{
		std::string level;

		switch (message.level)
		{
		case spdlog::level::info:
			level = "INFO";
			break;
		case spdlog::level::trace:
			level = "TRACE";
			break;
		case spdlog::level::warn:
			level = "WARN";
			break;
		case spdlog::level::err:
			level = "ERROR";
			break;
		case spdlog::level::critical:
			level = "FATAL";
			break;
		case spdlog::level::debug:
			level = "DEBUG";
			break;
		default:
			break;
		}

		destination.append(level.data(), level.data() + level.size());
	}

	std::unique_ptr<spdlog::custom_flag_formatter> UppercaseFormatter::clone() const
	{
		return spdlog::details::make_unique<UppercaseFormatter>();
	}

	void Log::Init()
	{
		auto formatter = CreateScope<spdlog::pattern_formatter>();
		formatter->add_flag<UppercaseFormatter>('*');
		formatter->set_pattern("%^[%T | %*]: %v%$");
		spdlog::set_formatter(std::move(formatter));

		s_Logger = spdlog::stdout_color_mt("Hyperion Server");
		s_Logger->set_level(spdlog::level::level_enum::trace);
		s_Logger->flush_on(spdlog::level::trace);
	}
}