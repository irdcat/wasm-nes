cmake . -B build/tests -G "Unix Makefiles"
cmake --build build/tests

cp ./tests/nestest.log ./build/tests/nestest.log
cp ./tests/nestest.nes ./build/tests/nestest.nes