#pragma once
#include "Core/Common/CommonHeaders.h"

namespace Nui
{
	/**
	* @brief Filesystem utility class with static methods
	* @note Acts as a wrapper for std::filesystem
	*/
	class Filesystem
	{
	public:
		/**
		* @brief Check if path exists
		* @param path Path to check
		* @return True if path exists
		*/
		static bool Exists(const fs::path& path);

		/**
		* @brief Check if path is a directory
		* @param path Path to check
		* @return True if path is a directory
		*/
		static bool IsDirectory(const fs::path& path);

		/**
		* @brief Check if path is a file
		* @param path Path to check
		* @return True if path is a file
		*/
		static bool IsFile(const fs::path& path);

		/**
		* @brief Create directory
		* @param path Path to create
		* @return True if directory was created or already exists
		*/
		static bool MakeDirectory(const fs::path& path);

		/**
		* @brief Delete directory
		* @param path Path to delete
		* @return True if directory was deleted
		*/
		static bool DeleteDirectory(const fs::path& path);

		/**
		* @brief Get current working directory
		* @return Current working directory
		* @note This is equivalent to fs::current_path()
		*/
		static fs::path GetCurrentWorkingDirectory();

		/**
		* @brief Get filename
		* @param path Path to get filename from
		* @param includeExtension Include file extension
		* @return Filename
		*/
		static String GetFilename(const fs::path& path, bool includeExtension = true);

		/**
		* @brief Get file extension
		* @param path Path to get extension from
		* @return File extension
		*/
		static String GetFileExtension(const fs::path& path);

		/**
		* @brief Get file size
		* @param path Path to get file size from
		* @return File size
		*/
		static U32Max GetFileSize(const fs::path& path);

		/**
		* @brief Read file as bytes
		* @param path Path to read
		* @return File contents
		*/
		static std::vector<Byte> ReadFileAsBytes(const fs::path& path);

		/**
		* @brief Read file as string
		* @param path Path to read
		* @return File contents as String
		*/
		static String ReadFileAsString(const fs::path& path);
	};
}  // namespace Nui