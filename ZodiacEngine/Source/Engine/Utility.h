#pragma once

namespace Zodiac
{
	namespace FileUtil {
		std::vector<char> ReadBinaryFile(const char* source);
		void WriteBinaryFile(const char* filename, const void* data, int size);

		bool ReadFile(const char* filename, std::string& outFile);
	}
}