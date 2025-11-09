#pragma once
#include <vulkan/vulkan.h>
#include <slang/slang.h>
#include <slang/slang-com-ptr.h>

namespace Zodiac {
	class ShaderCompiler {
	public:
		ShaderCompiler(const ShaderCompiler&) = delete;
		void operator=(ShaderCompiler const&) = delete;

		static ShaderCompiler& Get();

		~ShaderCompiler() = default;

		std::vector<uint32_t> CompileShaderFromText(VkDevice* device, const char* path);

	private:
		ShaderCompiler();

		bool CompileShader();
		bool LoadShaderProgram(VkDevice device);
		void SlangCreateGlobalSession();
		void DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob);
		void PrintEntrypointHashes(int entryPointCount, int targetCount, slang::IComponentType* composedProgram);

		Slang::ComPtr<slang::IGlobalSession> m_globalSession = nullptr;
		Slang::ComPtr<slang::ISession> m_slangSession = nullptr;
		uint64_t m_globalCounter = 0;
	};
}