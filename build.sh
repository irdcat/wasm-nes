set -e

emcmake cmake . -B build -G "Unix Makefiles"
cd build
make
cd ..