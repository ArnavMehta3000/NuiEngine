#include "Filesystem.h"
#include <fstream>

namespace Nui
{
	bool Filesystem::Exists(const fs::path& path)
	{
		return fs::exists(path);
	}

	bool Filesystem::IsDirectory(const fs::path& path)
	{
		return fs::is_directory(path);
	}

	bool Filesystem::IsFile(const fs::path& path)
	{
		return fs::is_regular_file(path);
	}

	bool Filesystem::MakeDirectory(const fs::path& path)
	{
		return fs::create_directories(path);
	}

	bool Filesystem::DeleteDirectory(const fs::path& path)
	{
		return fs::remove_all(path);
	}

	fs::path Filesystem::GetCurrentWorkingDirectory()
	{
		return fs::current_path();
	}

	String Filesystem::GetFilename(const fs::path& path, bool includeExtension)
	{
		return includeExtension ? path.filename().string() : path.filename().stem().string();
	}

	String Filesystem::GetFileExtension(const fs::path& path)
	{
		return path.extension().string();
	}

	U32Max Filesystem::GetFileSize(const fs::path& path)
	{
		return fs::file_size(path);
	}

	std::vector<Byte> Filesystem::ReadFileAsBytes(const fs::path& path)
	{
		std::ifstream file(path.string(), std::ios::binary);

		if (!file)
		{
			std::string message = "Failed to open file: " + path.string();
			NUI_LOG(Error, Filesystem, message);
			return std::vector<Byte>();
		}

		// Stop eating new lines in binary mode!!!
		file.unsetf(std::ios::skipws);

		return std::vector<Byte>(std::istreambuf_iterator<char>{file}, {});
	}

	String Filesystem::ReadFileAsString(const fs::path& path)
	{
		std::ifstream file(path.string());
		std::stringstream ss;
		ss << file.rdbuf();

		return ss.str();
	}
}  // namespace Nui