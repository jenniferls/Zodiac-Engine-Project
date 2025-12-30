#include <Zodiacpch.h>
#include "ShaderCompiler.h"

#include "SPIRV-Reflect/spirv_reflect.h"

#include "Utility.h"
#include "VulkanShaderModule.h"

Zodiac::ShaderCompiler& Zodiac::ShaderCompiler::Get() {
	static ShaderCompiler instance;
	return instance;
}

bool Zodiac::ShaderCompiler::CompileShaderFromText(VulkanDevice* device, const char* path) {
	SlangCreateSession();

	m_spirv = nullptr; //Clear previous compilation

	Slang::ComPtr<slang::IBlob> diagnosticsBlob;

	std::string source;
	if (!FileUtil::ReadFile(path, source)) {
		std::cout << "Failed to read shader source from file: " << path << std::endl;
		return false;
	}

	m_module = m_slangSession->loadModuleFromSourceString("test", path, source.c_str(), diagnosticsBlob.writeRef());
	//slang::IModule* module = m_slangSession->loadModule(path, diagnosticsBlob.writeRef());
	DiagnoseIfNeeded(diagnosticsBlob);
	if (!m_module) {
		std::cout << "Failed to load shader module: " << path << std::endl;
		return false;
	}

	const SlangInt32 definedEntryPointCount = m_module->getDefinedEntryPointCount();

	std::vector<Slang::ComPtr<slang::IEntryPoint>> entryPoints(definedEntryPointCount);
	std::vector<slang::IComponentType*> componentTypes(1 + definedEntryPointCount);
	componentTypes[0] = m_module;

	for (SlangInt32 i = 0; i < definedEntryPointCount; i++) {
		m_module->getDefinedEntryPoint(i, entryPoints[i].writeRef());
		componentTypes[1 + i] = entryPoints[i];
	}

	Slang::ComPtr<slang::IComponentType> composedProgram;
	SlangResult result = m_slangSession->createCompositeComponentType(componentTypes.data(), componentTypes.size(), composedProgram.writeRef(), diagnosticsBlob.writeRef());
	DiagnoseIfNeeded(diagnosticsBlob);
	if (SLANG_FAILED(result) || !composedProgram) {
		return false;
	}

	// From composite component type to linked program
	result = composedProgram->link(m_linkedProgram.writeRef(), diagnosticsBlob.writeRef());
	DiagnoseIfNeeded(diagnosticsBlob);
	if (SLANG_FAILED(result) || !m_linkedProgram) {
		return false;
	}

	// From linked program to SPIR-V
	result = m_linkedProgram->getTargetCode(0, m_spirv.writeRef(), diagnosticsBlob.writeRef());
	DiagnoseIfNeeded(diagnosticsBlob);
	if (SLANG_FAILED(result) || nullptr == m_spirv) {
		return false;
	}

	return true;
}

const uint32_t* Zodiac::ShaderCompiler::GetSPIRV() {
	if (!m_spirv) {
		return nullptr;
	}
	return reinterpret_cast<const uint32_t*>(m_spirv->getBufferPointer());
}

size_t Zodiac::ShaderCompiler::GetSPIRVSize() {
	if (!m_spirv) {
		return 0;
	}
	return m_spirv->getBufferSize();
}

void Zodiac::ShaderCompiler::AddDefaultTarget() {
	m_targets.push_back({
		.format = SLANG_SPIRV,
		.profile = m_globalSession->findProfile("spirv_1_6+vulkan_1_4"),
		.flags = SLANG_TARGET_FLAG_GENERATE_SPIRV_DIRECTLY,
		.forceGLSLScalarBufferLayout = true, //TODO: Need to check documentation
	});
}

void Zodiac::ShaderCompiler::AddDefaultOptions() {
	m_options.push_back({ slang::CompilerOptionName::EmitSpirvDirectly, {slang::CompilerOptionValueKind::Int, 1} });
	m_options.push_back({ slang::CompilerOptionName::VulkanUseEntryPointName, {slang::CompilerOptionValueKind::Int, 1} });
	m_options.push_back({ slang::CompilerOptionName::VulkanInvertY, {slang::CompilerOptionValueKind::Int, 1} });
	m_options.push_back({ slang::CompilerOptionName::VulkanUseGLLayout, {slang::CompilerOptionValueKind::Int, 1} });
}

void Zodiac::ShaderCompiler::AddTarget(const slang::TargetDesc& target) {
	m_targets.push_back(target);
}

void Zodiac::ShaderCompiler::ClearTargets() {
	m_targets.clear();
}

slang::IComponentType* Zodiac::ShaderCompiler::GetLinkedProgram() {
	if (!m_linkedProgram) {
		return nullptr;
	}
	return m_linkedProgram.get();
}

Zodiac::ShaderCompiler::ShaderCompiler() {
	SlangCreateGlobalSession();
	AddDefaultTarget();
	AddDefaultOptions();
}

bool Zodiac::ShaderCompiler::CompileShader() {
	return false;
}

bool Zodiac::ShaderCompiler::LoadShaderProgram(VkDevice device) {
	return false;
}

void Zodiac::ShaderCompiler::SlangCreateGlobalSession() {
	slang::createGlobalSession(m_globalSession.writeRef());
}

void Zodiac::ShaderCompiler::SlangCreateSession() {
	m_slangSession = {};

	slang::SessionDesc sessionDesc = {};
	sessionDesc.targets = m_targets.data();
	sessionDesc.targetCount = SlangInt(m_targets.size());
	sessionDesc.searchPaths = m_searchPaths.data();
	sessionDesc.searchPathCount = SlangInt(m_searchPaths.size());
	sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR; //I hate it here
	sessionDesc.preprocessorMacros = m_macros.data();
	sessionDesc.preprocessorMacroCount = SlangInt(m_macros.size());
	sessionDesc.allowGLSLSyntax = true;
	sessionDesc.compilerOptionEntries = m_options.data();
	sessionDesc.compilerOptionEntryCount = uint32_t(m_options.size());

	m_globalSession->createSession(sessionDesc, m_slangSession.writeRef());
}

void Zodiac::ShaderCompiler::DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob) {
	if (diagnosticsBlob != nullptr) {
		printf("%s", (const char*)diagnosticsBlob->getBufferPointer());
	}
}

void Zodiac::ShaderCompiler::PrintEntrypointHashes(int entryPointCount, int targetCount, slang::IComponentType* composedProgram) {
	for (int targetIndex = 0; targetIndex < targetCount; targetIndex++) {
		for (int entryPointIndex = 0; entryPointIndex < entryPointCount; entryPointIndex++) {
			Slang::ComPtr<slang::IBlob> entryPointHashBlob;
			composedProgram->getEntryPointHash(entryPointIndex, targetIndex, entryPointHashBlob.writeRef());
			std::cout << "callIdx: " << m_globalCounter << ", entrypoint: " << entryPointIndex << ", target: " << targetIndex << ", hash: " << std::endl;
			m_globalCounter++;

			uint8_t* buffer = (uint8_t*)entryPointHashBlob->getBufferPointer();
			for (size_t i = 0; i < entryPointHashBlob->getBufferSize(); i++) {
				std::cout << std::format("%.2X", buffer[i]) << std::endl;
			}
		}
	}
}

VkShaderStageFlagBits Zodiac::ShaderCompiler::SlangStageToVulkanShaderStage(SlangStage stage) {
	switch (stage) {
	case SlangStage::SLANG_STAGE_VERTEX:
		return VK_SHADER_STAGE_VERTEX_BIT;
	case SlangStage::SLANG_STAGE_FRAGMENT:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case SlangStage::SLANG_STAGE_COMPUTE:
		return VK_SHADER_STAGE_COMPUTE_BIT;
	case SlangStage::SLANG_STAGE_HULL:
		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	case SlangStage::SLANG_STAGE_DOMAIN:
		return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	case SlangStage::SLANG_STAGE_GEOMETRY:
		return VK_SHADER_STAGE_GEOMETRY_BIT;
	case SlangStage::SLANG_STAGE_RAY_GENERATION:
		return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
	case SlangStage::SLANG_STAGE_INTERSECTION:
		return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
	case SlangStage::SLANG_STAGE_ANY_HIT:
		return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
	case SlangStage::SLANG_STAGE_CLOSEST_HIT:
		return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
	case SlangStage::SLANG_STAGE_MISS:
		return VK_SHADER_STAGE_MISS_BIT_KHR;
	case SlangStage::SLANG_STAGE_CALLABLE:
		return VK_SHADER_STAGE_CALLABLE_BIT_KHR;
	case SlangStage::SLANG_STAGE_MESH:
		return VK_SHADER_STAGE_MESH_BIT_EXT;
	case SlangStage::SLANG_STAGE_AMPLIFICATION:
		return VK_SHADER_STAGE_TASK_BIT_EXT;
	default:
		std::cout << ("Unimplemented or unknown stage %u!\n", static_cast<unsigned>(stage)) << std::endl;
		return static_cast<VkShaderStageFlagBits>(0);
	}
}
