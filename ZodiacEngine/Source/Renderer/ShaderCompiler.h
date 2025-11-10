#pragma once
#include <vulkan/vulkan.h>
#include <slang/slang.h>
#include <slang/slang-com-ptr.h>
#include "VulkanDevice.h"

namespace Zodiac {
	class ShaderCompiler {
	public:
		ShaderCompiler(const ShaderCompiler&) = delete;
		void operator=(ShaderCompiler const&) = delete;

		static ShaderCompiler& Get();

		~ShaderCompiler() = default;

		bool CompileShaderFromText(VulkanDevice* device, const char* path);
		const uint32_t* GetSPIRV();
		size_t GetSPIRVSize();
		void AddDefaultTarget();
		void AddTarget(const slang::TargetDesc& target);
		void ClearTargets();

	private:
		ShaderCompiler();

		bool CompileShader();
		bool LoadShaderProgram(VkDevice device);
		void SlangCreateGlobalSession();
		void SlangCreateSession();
		void DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob);
		void PrintEntrypointHashes(int entryPointCount, int targetCount, slang::IComponentType* composedProgram);
		static VkShaderStageFlagBits SlangStageToVulkanShaderStage(SlangStage stage);

		uint64_t m_globalCounter = 0;
		Slang::ComPtr<slang::IGlobalSession> m_globalSession = nullptr;
		Slang::ComPtr<slang::ISession> m_slangSession = nullptr;
		Slang::ComPtr<ISlangBlob> m_spirv = nullptr;
		Slang::ComPtr<slang::IModule> m_module = nullptr;
		Slang::ComPtr<slang::IComponentType> m_linkedProgram = nullptr;
		std::vector<slang::PreprocessorMacroDesc> m_macros;
		std::vector<slang::CompilerOptionEntry> m_options;
		std::vector<slang::TargetDesc> m_targets;
		std::vector<const char*> m_searchPaths;
	};
}