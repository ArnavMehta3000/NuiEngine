#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <filesystem>
#include <stacktrace>
#include <chrono>

namespace Nui
{
	using I8 = int8_t;
	using I16 = int16_t;
	using I32 = int32_t;
	using I64 = int64_t;

	using U8  = uint8_t;
	using U16 = uint16_t;
	using U32 = uint32_t;
	using U64 = uint64_t;

	using F32 = float;
	using F64 = double;

	using Byte = uint8_t;

	using String = std::string;
	using StringView = std::string_view;

	using StringW = std::wstring;
	using StringWView = std::wstring_view;
	
	using Stacktrace  = std::stacktrace;

	namespace fs     = std::filesystem;
	namespace chrono = std::chrono;
}