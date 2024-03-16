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

	using U32Max = uintmax_t;
	using I32Max = intmax_t;

	constexpr U64 U64InvalidId = 0xffff'ffff'ffff'ffffuI64;
	constexpr U32 U32InvalidId = 0xffff'ffffuI32;
	constexpr U16 U16InvalidId = 0xffffuI16;
	constexpr U8  U8InvalidId  = 0xffuI8;

	using F32 = float;
	using F64 = double;

	using Byte = uint8_t;

	using String = std::string;
	using StringView = std::string_view;

	using StringW = std::wstring;
	using StringViewW = std::wstring_view;
	
	using Stacktrace  = std::stacktrace;

	namespace fs     = std::filesystem;
	namespace chrono = std::chrono;
}