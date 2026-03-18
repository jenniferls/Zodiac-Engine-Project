set -e

mkdir -p build
cd build

export CC=clang
export CCX=clang++

cmake .. -G Xcode

cmake --build . --config Debug

echo "Build completed"

read -p "Press enter to continue.."
