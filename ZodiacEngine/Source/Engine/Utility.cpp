#include <Zodiacpch.h>
#include "Utility.h"

std::vector<char> Zodiac::FileUtil::ReadBinaryFile(const char* source) {
	std::ifstream file(source, std::ios::binary);
	if (!file.fail()) {
		std::streampos begin, end;
		begin = file.tellg();
		file.seekg(0, std::ios::end);
		end = file.tellg();

		std::vector<char> result(static_cast<size_t>(end - begin));
		file.seekg(0, std::ios::beg);
		file.read(&result[0], end - begin);
		file.close();
		return result;
	}

	std::cout << "Could not open \"" << source << "\" file!" << std::endl;
	return std::vector<char>();
}

void Zodiac::FileUtil::WriteBinaryFile(const char* filename, const void* data, int size)
{
	FILE* file = fopen(filename, "wb");
	fwrite(data, sizeof(uint8_t), size, file);
	fclose(file);
}

bool Zodiac::FileUtil::ReadFile(const char* filename, std::string& outFile)
{
	std::ifstream file(filename);

	if (!file.fail()) {
		std::string line;
		while (std::getline(file, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		file.close();
		return true;
	}
	
	std::cout << "Could not open file: " << filename << std::endl;

	return false;
}