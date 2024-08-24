set -ex

emcmake cmake . -B build/tests -G "Unix Makefiles" -DTESTS:BOOLEAN=true

mkdir -p ./build/tests/resources
cp -r ./tests/resources/* ./build/tests/resources
cd build/tests
make
cd ../..