#include "Zodiacpch.h"
#include "FileWatcher.h"

void Zodiac::FileWatcher::WatchFile(const std::string& filePath)
{
	if (!std::filesystem::exists(filePath) ) {
	std::cout << "FileWatcher: File does not exist: " << filePath << std::endl;
		return;
	}

	FileEntry entry;
	entry.lastModified = std::filesystem::last_write_time(filePath);
	entry.changed = false;
	m_files.emplace(filePath, std::move(entry));
	std::cout << "FileWatcher: Now watching file: " << filePath << std::endl;
}

bool Zodiac::FileWatcher::Poll()
{
	bool filesChanged = false;

	for (auto& it : m_files) {
		const std::string& filePath = it.first;
		FileEntry& entry = it.second;

		if (!std::filesystem::exists(filePath)) {
			continue;
		}

		auto currentModifiedTime = std::filesystem::last_write_time(filePath);
		if (currentModifiedTime != entry.lastModified) {
			entry.lastModified = currentModifiedTime;
			entry.changed = true;
			filesChanged = true;
			std::cout << "FileWatcher: Detected change in file: " << filePath << std::endl;
		}
	}
	return filesChanged;
}

bool Zodiac::FileWatcher::HasFileChanged(const std::string& filePath)
{
	auto it = m_files.find(filePath);
	return it != m_files.end() && it->second.changed;
}

void Zodiac::FileWatcher::ResetFlags()
{
	for (auto& keyValue : m_files) {
		keyValue.second.changed = false;
	}
}
