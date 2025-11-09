#include <Zodiacpch.h>
#include "ShaderCompiler.h"

#include "SPIRV-Reflect/spirv_reflect.h"

#include "Utility.h"

Zodiac::ShaderCompiler& Zodiac::ShaderCompiler::Get() {
	static ShaderCompiler instance;
	return instance;
}

std::vector<uint32_t> Zodiac::ShaderCompiler::CompileShaderFromText(VkDevice* device, const char* path)
{
	Slang::ComPtr<slang::IBlob> diagnosticsBlob;

	slang::IModule* module = m_slangSession->loadModule(path, diagnosticsBlob.writeRef());
	DiagnoseIfNeeded(diagnosticsBlob);
	if (!module) {
		std::cout << "Failed to load shader module: " << path << std::endl;
		return std::vector<uint32_t>();
	}

	//TODO: Should support all types in shaders in the future
	Slang::ComPtr<slang::IEntryPoint> vertexEntryPoint;
	module->findEntryPointByName("vertexMain", vertexEntryPoint.writeRef());
	Slang::ComPtr<slang::IEntryPoint> fragmentEntryPoint;
	module->findEntryPointByName("fragmentMain", fragmentEntryPoint.writeRef());

	std::vector<slang::IComponentType*> componentTypes;
	componentTypes.push_back(module);

	int entryPointCount = 0;
	int vertexEntryPointIndex = entryPointCount++;
	componentTypes.push_back(vertexEntryPoint);

	int fragmentEntryPointIndex = entryPointCount++;
	componentTypes.push_back(fragmentEntryPoint);

	Slang::ComPtr<slang::IComponentType> linkedProgram;
	SlangResult result = m_slangSession->createCompositeComponentType(componentTypes.data(), componentTypes.size(), linkedProgram.writeRef(), diagnosticsBlob.writeRef());
	DiagnoseIfNeeded(diagnosticsBlob);

	//PrintEntrypointHashes(entryPointCount, 1, linkedProgram);

	return std::vector<uint32_t>();
}

Zodiac::ShaderCompiler::ShaderCompiler()
{
	SlangCreateGlobalSession();
	slang::SessionDesc sessionDesc = {};
	m_globalSession->createSession(sessionDesc, m_slangSession.writeRef());
}

bool Zodiac::ShaderCompiler::CompileShader()
{
	return false;
}

bool Zodiac::ShaderCompiler::LoadShaderProgram(VkDevice device)
{
	return false;
}

void Zodiac::ShaderCompiler::SlangCreateGlobalSession()
{
	slang::createGlobalSession(m_globalSession.writeRef());
}

void Zodiac::ShaderCompiler::DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob)
{
	if (diagnosticsBlob != nullptr)
	{
		printf("%s", (const char*)diagnosticsBlob->getBufferPointer());
	}
}

void Zodiac::ShaderCompiler::PrintEntrypointHashes(int entryPointCount, int targetCount, slang::IComponentType* composedProgram)
{
	for (int targetIndex = 0; targetIndex < targetCount; targetIndex++)
	{
		for (int entryPointIndex = 0; entryPointIndex < entryPointCount; entryPointIndex++)
		{
			Slang::ComPtr<slang::IBlob> entryPointHashBlob;
			composedProgram->getEntryPointHash(entryPointIndex, targetIndex, entryPointHashBlob.writeRef());
			std::cout << "callIdx: " << m_globalCounter << ", entrypoint: " << entryPointIndex << ", target: " << targetIndex << ", hash: " << std::endl;
			m_globalCounter++;

			uint8_t* buffer = (uint8_t*)entryPointHashBlob->getBufferPointer();
			for (size_t i = 0; i < entryPointHashBlob->getBufferSize(); i++)
			{
				std::cout << std::format("%.2X", buffer[i]) << std::endl;
			}
		}
	}
}
