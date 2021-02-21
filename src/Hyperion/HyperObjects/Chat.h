#pragma once

#include <optional>
#include <string>

struct Chat
{
	std::string Text;
	std::optional<bool> Bold;
	std::optional<bool> Italic;
	std::optional<bool> Underlined;
	std::optional<bool> Strikethrough;
	std::optional<bool> Obfuscated;
};
