cmake_minimum_required(VERSION 3.22)
project(wasm-nes)

if(EMSCRIPTEN)
    set(APP_SOURCES
        src/Cpu.cpp
        src/Mmu.cpp
        src/Ppu.cpp
        src/Cartridge.cpp
        src/Emulator.cpp
        src/PpuFramebuffer.cpp
        src/SdlColor.cpp
        src/main.cpp)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sWASM=1 -sALLOW_MEMORY_GROWTH=1 -sUSE_SDL=2 -sFORCE_FILESYSTEM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sEXPORTED_FUNCTIONS=[_main,_loadRom] -sEXPORTED_RUNTIME_METHODS=ccall")
    
    add_executable(wasm-nes ${APP_SOURCES})
endif()

if(NOT EMSCRIPTEN)
    set(APP_LIB_SOURCES
        src/Cpu.cpp
        src/Mmu.cpp
        src/Ppu.cpp
        src/Cartridge.cpp
        src/PpuFramebuffer.cpp)
    set(TEST_SOURCES
        tests/util/SystemUnderTest.cpp
        tests/util/IntegrationTest.cpp
        tests/util/Test.cpp
        tests/util/TestExecutor.cpp
        tests/main.cpp)
    set(CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")

    add_library(wasm-nes ${APP_LIB_SOURCES})
    add_executable(wasm-nes-tests ${TEST_SOURCES})
    target_link_libraries(wasm-nes-tests wasm-nes)
endif()