set -e

emcmake cmake . -B build -G "Unix Makefiles"

cp ./src/web/index.html ./build
cp ./src/web/index.js ./build
npm install
npx tailwindcss -i ./src/web/index.css -o ./build/index.css --minify

cd build
make
cd ..