cmake_minimum_required(VERSION 3.22)
project(wasm-nes)

if(EMSCRIPTEN)
    set(APP_SOURCES
        src/Cpu.cpp
        src/Mmu.cpp)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -sWASM=1 -sALLOW_MEMORY_GROWTH=1")
    
    add_executable(wasm-nes ${APP_SOURCES})
endif()

if(NOT EMSCRIPTEN)
    set(APP_LIB_SOURCES
        src/Cpu.cpp
        src/Mmu.cpp)
    set(TEST_SOURCES
        tests/CpuTest.cpp)
    set(CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

    add_library(wasm-nes ${APP_LIB_SOURCES})
    add_executable(wasm-nes-tests ${TEST_SOURCES})
    target_link_libraries(wasm-nes-tests wasm-nes)
endif()