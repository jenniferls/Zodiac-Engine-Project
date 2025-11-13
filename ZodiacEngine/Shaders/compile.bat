@echo off
set SLANGC="C:/VulkanSDK/1.4.328.1/Bin/slangc.exe"
set SHADER="test.slang"

%SLANGC% %SHADER% -target spirv -entry vertexMain -stage vertex -entry fragmentMain -stage fragment -o triangle.spv

pause