@echo off

mkdir build
cd build

cmake .. -G "Visual Studio 18 2026" -A x64

cmake --build . --config Debug

pause