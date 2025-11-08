@echo off
set SLANGC="C:/VulkanSDK/1.4.321.1/Bin/slangc.exe"
set SHADER="test.slang"

%SLANGC% %SHADER% -target spirv -stage vertex -entry vertexMain -o triangle.vert.spv
%SLANGC% %SHADER% -target spirv -stage fragment -entry fragmentMain -o triangle.frag.spv

pause