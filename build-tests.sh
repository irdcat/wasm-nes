set -e

cmake . -B build/tests -G "Unix Makefiles"
cmake --build build/tests

cp ./tests/resources/*.nes ./build/tests