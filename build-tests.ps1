cmake . -B build/tests -G "Unix Makefiles"
cmake --build build/tests

$copyExclude = Get-ChildItem -Recurse .\build\tests
Copy-Item .\tests\nestest.log -Destination .\build\tests -Exclude $copyExclude
Copy-Item .\tests\nestest.nes -Destination .\build\tests -Exclude $copyExclude