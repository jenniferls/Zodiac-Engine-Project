#pragma once

namespace Zodiac {
	class Shader {
	public:
		Shader() = default;
		~Shader() = default;
	private:
		std::string m_path = "";
	};
}