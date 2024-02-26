set -e

cmake . -B build/tests -G "Unix Makefiles"
cmake --build build/tests

cp ./tests/resources/nestest.log ./build/tests/nestest.log
cp ./tests/resources/*.nes ./build/tests