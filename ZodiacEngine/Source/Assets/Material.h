#pragma once

namespace Zodiac {
	class Material {
	public:
		Material() = default;
		~Material() = default;
	private:
		std::string name = "";
		std::vector<uint32_t> m_textureIndices;
	};
}