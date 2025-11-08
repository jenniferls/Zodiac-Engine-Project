#include <Zodiacpch.h>
#include "ShaderCompiler.h"

#include <slang/slang-com-ptr.h>
#include "glslang/Public/resource_limits_c.h"
#include "SPIRV-Reflect/spirv_reflect.h"

#include "Utility.h"

std::vector<uint32_t> Zodiac::ShaderCompiler::CompileShaderFromText(VkDevice device, const char* path)
{
	//std::string source;

	//if (!FileUtil::ReadFile(path, source)) {
	//	std::cout << "Failed to read shader source from file: " << path << std::endl;
	//}

	Slang::ComPtr<slang::ISession> slangSession; //Should be stored somewhere?
	//slangSession = device->getSlangSession();
	Slang::ComPtr<slang::IBlob> diagnosticsBlob;

	slang::IModule* module = slangSession->loadModule(path, diagnosticsBlob.writeRef());
	DiagnoseIfNeeded(diagnosticsBlob);
	if (!module) {
		std::cout << "Failed to load shader module: " << path << std::endl;
		return std::vector<uint32_t>();
	}

	//glslang_initialize_process();

	//bool success = CompileShader();

	//glslang_finalize_process();

	return std::vector<uint32_t>();
}

bool Zodiac::ShaderCompiler::CompileShader()
{
	return false;
}

bool Zodiac::ShaderCompiler::LoadShaderProgram(VkDevice device)
{
	return false;
}

glslang_stage_t Zodiac::ShaderCompiler::GetShaderStageFromFilename(const char* filename)
{
	std::string s(filename);

	return glslang_stage_t();
}

void Zodiac::ShaderCompiler::DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob)
{
	if (diagnosticsBlob != nullptr)
	{
		printf("%s", (const char*)diagnosticsBlob->getBufferPointer());
	}
}
