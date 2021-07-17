/*
 * Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <Utilities/Prerequisites.hpp>
#include <fmt/format.h>
#include <string_view>

class Logger
{
public:
	NON_COPYABLE(Logger);
	NON_MOVEABLE(Logger);
	
public:
	enum class Level
	{
		Info,
		Warning,
		Error,
		Fatal,
		Debug
	};
	
public:
	constexpr Logger() = delete;
	~Logger() = delete;
	
	template <typename... Args>
	static void info(std::string_view format, Args&&... args)
	{
		log(Level::Info, format, std::forward<Args>(args)...);
	}
	
	template <typename... Args>
	static void warning(std::string_view format, Args&&... args)
	{
		log(Level::Warning, format, std::forward<Args>(args)...);
	}
	
	template <typename... Args>
	static void error(std::string_view format, Args&&... args)
	{
		log(Level::Error, format, std::forward<Args>(args)...);
	}
	
	template <typename... Args>
	static void fatal(std::string_view format, Args&&... args)
	{
		log(Level::Fatal, format, std::forward<Args>(args)...);
	}
	
	template <typename... Args>
	static void debug(std::string_view format, Args&&... args)
	{
		log(Level::Debug, format, std::forward<Args>(args)...);
	}

private:
	template <typename... Args>
	static void log(Level level, std::string_view format, Args&&... args)
	{
		if (format.empty())
		{
			internal_log(level, format);
			return;
		}
		
		static constexpr const size_t args_count = sizeof...(Args);
		if constexpr (args_count == 0)
		{
			internal_log(level, format);
			return;
		}
		
		internal_log(level, fmt::format(format, std::forward<Args>(args)...));
	}
	
	static void internal_log(Level level, std::string_view string);
};
