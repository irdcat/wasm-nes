cmake_minimum_required(VERSION 3.22)
project(wasm-nes)

if(NOT TESTS)
    set(APP_SOURCES
        src/core/Cpu.cpp
        src/core/Mmu.cpp
        src/core/Ppu.cpp
        src/core/Cartridge.cpp
        src/core/Mapper.cpp
        src/core/Mapper0.cpp
        src/core/Mapper2.cpp
        src/core/Mapper3.cpp
        src/core/Mapper7.cpp
        src/core/Controllers.cpp
        src/Emulator.cpp
        src/main.cpp)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sWASM=1 -sASYNCIFY -sALLOW_MEMORY_GROWTH=1 -sUSE_SDL=2 -sFORCE_FILESYSTEM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sEXPORTED_FUNCTIONS=_run,_loadRom -sEXPORTED_RUNTIME_METHODS=ccall")
    
    add_executable(wasm-nes ${APP_SOURCES})
endif()

if(TESTS)
    set(TEST_RESOURCES_DIR "./resources/")
    set(APP_LIB_SOURCES
        src/core/Cpu.cpp
        src/core/Mmu.cpp
        src/core/Ppu.cpp
        src/core/Cartridge.cpp
        src/core/Mapper.cpp
        src/core/Mapper0.cpp
        src/core/Mapper2.cpp
        src/core/Mapper3.cpp
        src/core/Mapper7.cpp
        src/core/Controllers.cpp)
    set(TEST_SOURCES
        tests/util/SystemUnderTest.cpp
        tests/util/NesTestLogParser.cpp
        tests/CpuInstructionsTest.cpp)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${TEST_RESOURCES_DIR}")

    include(FetchContent)
    FetchContent_Declare(
        google-test
        URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip)
    FetchContent_MakeAvailable(google-test)

    add_library(wasm-nes ${APP_LIB_SOURCES})
    add_executable(wasm-nes-tests ${TEST_SOURCES})
    target_link_libraries(
        wasm-nes-tests 
        wasm-nes
        GTest::gtest_main)
endif()