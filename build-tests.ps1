cmake . -B build/tests -G "Unix Makefiles"
cmake --build build/tests

$copyExclude = Get-ChildItem -Recurse .\build\tests
Copy-Item .\tests\resources\*.nes -Destination .\build\tests -Exclude $copyExclude
Copy-Item .\tests\resources\*.log -Destination .\build\tests -Exclude $copyExclude