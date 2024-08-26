$ErrorActionPreference = "Stop"

emcmake cmake . -B build/tests -G "Unix Makefiles" -DTESTS:BOOLEAN=true

New-Item -ItemType Directory -Force -Path .\build\tests\resources
$copyExclude = Get-ChildItem -Recurse .\build\tests\resources
Copy-Item .\tests\resources\ -Destination -Recurse .\build\tests\resources -Exclude $copyExclude
cd build\tests
make
cd ..\..