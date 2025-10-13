set -e 

cmake -S . -B build/ -DGLFW_BUILD_DOCS=OFF

cd build

make
./NanoCraft
