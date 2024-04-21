set -e

cmake . -B build/tests -G "Unix Makefiles" -DTESTS:BOOLEAN=true
cmake --build build/tests

cp ./tests/resources/*.nes ./build/tests