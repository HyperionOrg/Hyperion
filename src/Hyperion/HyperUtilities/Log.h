#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/pattern_formatter.h>

#include "Hyperion/Core.h"

namespace Hyperion
{
	class UppercaseFormatter : public spdlog::custom_flag_formatter
	{
	public:
		void format(const spdlog::details::log_msg& message, const std::tm& time, spdlog::memory_buf_t& destination) override;

		virtual std::unique_ptr<custom_flag_formatter> clone() const override;
	};

	class Log
	{
	private:
		static Ref<spdlog::logger> s_Logger;

	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetLogger() { return s_Logger; }
	};
}

#define HP_INFO(...) ::Hyperion::Log::GetLogger()->info(__VA_ARGS__)
#define HP_TRACE(...) ::Hyperion::Log::GetLogger()->trace(__VA_ARGS__)
#define HP_WARN(...) ::Hyperion::Log::GetLogger()->warn(__VA_ARGS__)
#define HP_ERROR(...) ::Hyperion::Log::GetLogger()->error(__VA_ARGS__)
#define HP_FATAL(...) ::Hyperion::Log::GetLogger()->critical(__VA_ARGS__)
#define HP_DEBUG(...) ::Hyperion::Log::GetLogger()->debug(__VA_ARGS__)
