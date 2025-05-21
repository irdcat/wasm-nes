cmake_minimum_required(VERSION 3.22)
project(wasm-nes)

if(NOT TESTS)
    set(WASM_NES_SOURCES
        src/core/Cpu.cpp
        src/core/Mmu.cpp
        src/core/Ppu.cpp
        src/core/Apu.cpp
        src/core/apu/AudioChannel.cpp
        src/core/apu/PulseChannel.cpp
        src/core/Cartridge.cpp
        src/core/mapper/Mapper.cpp
        src/core/mapper/Mapper0.cpp
        src/core/mapper/Mapper1.cpp
        src/core/mapper/Mapper2.cpp
        src/core/mapper/Mapper3.cpp
        src/core/mapper/Mapper7.cpp
        src/core/Controllers.cpp
        src/Emulator.cpp
        src/main.cpp)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(WASM_NES_COMPILE_OPTIONS
        -std=c++20
        -O3
        --use-port=sdl2)
    set(WASM_NES_LINK_OPTIONS
        -sWASM=1
        -sASSERTIONS=0
        -sASYNCIFY
        -sALLOW_MEMORY_GROWTH
        --use-port=sdl2
        -sFORCE_FILESYSTEM=1
        -sEXPORTED_FUNCTIONS=_run,_loadRom
        -sEXPORTED_RUNTIME_METHODS=ccall)
    add_executable(wasm-nes ${WASM_NES_SOURCES})
    target_compile_options(wasm-nes PUBLIC ${WASM_NES_COMPILE_OPTIONS})
    target_link_options(wasm-nes PUBLIC ${WASM_NES_LINK_OPTIONS})
endif()

if(TESTS)
    set(TEST_RESOURCES_DIR "./resources/")
    set(WASM_NES_SOURCES
        src/core/Cpu.cpp
        src/core/Mmu.cpp
        src/core/Ppu.cpp
        src/core/Apu.cpp
        src/core/apu/AudioChannel.cpp
        src/core/apu/PulseChannel.cpp
        src/core/Cartridge.cpp
        src/core/mapper/Mapper.cpp
        src/core/mapper/Mapper0.cpp
        src/core/mapper/Mapper1.cpp
        src/core/mapper/Mapper2.cpp
        src/core/mapper/Mapper3.cpp
        src/core/mapper/Mapper7.cpp
        src/core/Controllers.cpp)
    set(WASM_NES_TESTS_SOURCES
        tests/util/SystemUnderTest.cpp
        tests/util/NesTestLogParser.cpp
        tests/util/GenericRomTest.cpp
        tests/util/BlarggRomTest.cpp
        tests/CpuInstructionsTest.cpp
        tests/CpuInstructionsTestV5.cpp
        tests/CpuInstructionTimingTest.cpp
        tests/CpuInterruptsTest.cpp
        tests/CpuMiscTest.cpp
        tests/CpuResetTest.cpp
        tests/PpuGeneralTest.cpp
        tests/PpuOamTest.cpp
        tests/PpuOpenBusTest.cpp
        tests/PpuSpriteHitTest.cpp
        tests/PpuVblankNmiTest.cpp)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(WASM_NES_COMPILE_OPTIONS
        -std=c++20)
    set(WASM_NES_LINK_OPTIONS
        --preload-file ${TEST_RESOURCES_DIR})

    include(FetchContent)
    FetchContent_Declare(
        google-test
        URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip)
    FetchContent_MakeAvailable(google-test)

    add_library(wasm-nes ${WASM_NES_SOURCES})
    target_compile_options(wasm-nes PUBLIC ${WASM_NES_COMPILE_OPTIONS})
    target_link_options(wasm-nes PUBLIC ${WASM_NES_LINK_OPTIONS})
    add_executable(wasm-nes-tests ${WASM_NES_TESTS_SOURCES})
    target_link_libraries(
        wasm-nes-tests 
        wasm-nes
        GTest::gtest_main)
endif()