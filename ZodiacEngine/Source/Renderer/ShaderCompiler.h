#pragma once
#include <vulkan/vulkan.h>
#include <slang/slang.h>
#include "glslang/Include/glslang_c_interface.h"

namespace Zodiac {
	class ShaderCompiler {
	public:
		ShaderCompiler() = default;
		~ShaderCompiler() = default;

		std::vector<uint32_t> CompileShaderFromText(VkDevice device, const char* path);

	private:
		bool CompileShader();
		bool LoadShaderProgram(VkDevice device);
		static glslang_stage_t GetShaderStageFromFilename(const char* filename);
		void DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob);
	};
}