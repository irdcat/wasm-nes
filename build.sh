set -e

emcmake cmake . -B build -G "Unix Makefiles"

cp ./src/web/index.html ./build
cp ./src/web/index.js ./build

cd build
make
cd ..