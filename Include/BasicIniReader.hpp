//----------------------------------------------------------------------------------------------
// Basic ini reader
// Copyright (c) 2023-2024 Kitsuune - Apache License 2.0
//

#pragma once

#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <span>
#include <cassert>

namespace ini
{
	struct KeyValuePair
	{
		std::shared_ptr<std::string> Section;
		std::string Key;
		std::string Value;
	};

	std::vector<KeyValuePair> LoadFromFile(const std::filesystem::path& Path);

	template<typename T> requires (std::is_arithmetic_v<T> || std::is_same_v<T, std::string_view>)
	std::optional<T> GetValue(std::span<const KeyValuePair> Entries, std::string_view Section, std::string_view Key)
	{
		const std::string* ValueStr = nullptr;
		for(auto& Entry : Entries)
		{
			assert(Entry.Section);
			if(*Entry.Section == Section && Entry.Key == Key)
			{
				ValueStr = &Entry.Value;
				break;
			}
		}

		if(!ValueStr) return {};
		if constexpr(std::is_same_v<T, bool>)
		{
			if(*ValueStr == "1" || _stricmp(ValueStr->c_str(), "true")  == 0) return true;
			if(*ValueStr == "0" || _stricmp(ValueStr->c_str(), "false") == 0) return false;
			return {};
		}
		else if constexpr(std::is_integral_v<T>)
		{
			char* End = nullptr;
			auto Value = strtoll(ValueStr->c_str(), &End, 0);
			if(End && End != ValueStr->c_str())
			{
				if constexpr(std::is_unsigned_v<T>)
					if(ValueStr->front() == '-') Value = 0;
				
				return static_cast<T>(Value);
			}
			return {};
		}
		else if constexpr(std::is_floating_point_v<T>)
		{
			char* End = nullptr;
			auto Value = strtod(ValueStr->c_str(), &End);
			if(End && End != ValueStr->c_str()) return static_cast<T>(Value);
			return {};
		}
		else return *ValueStr;
	}
}
