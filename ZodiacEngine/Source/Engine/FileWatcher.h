#pragma once
#include <filesystem>
#include <unordered_map>

namespace Zodiac {
	class FileWatcher {
	public:
		FileWatcher() = default;
		~FileWatcher() = default;

		void WatchFile(const std::string& filePath);
		bool Poll();
		bool HasFileChanged(const std::string& filePath);
		void ResetFlags();

	private:
		struct FileEntry {
			std::filesystem::file_time_type lastModified;
			bool changed = false;
		};

		std::unordered_map<std::string, FileEntry> m_files;
	};
}