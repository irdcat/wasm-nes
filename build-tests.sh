set -e

cmake . -B build/tests -G "Unix Makefiles"
cmake --build build/tests

cp ./tests/resources/nestest.log ./build/tests/nestest.log
cp ./tests/resources/nestest.nes ./build/tests/nestest.nes
cp ./tests/resources/ppu_vbl_nmi.nes ./build/tests/ppu_vbl_nmi.nes
cp ./tests/resources/ppu_open_bus.nes ./build/tests/ppu_open_bus.nes
cp ./tests/resources/test_ppu_read_buffer.nes ./build/tests/test_ppu_read_buffer.nes