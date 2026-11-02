cmake_minimum_required(VERSION 3.21)
project(HuffmanLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Статична бібліотека
add_library(HuffmanLib STATIC
    src/HuffmanCodec.cpp
)

target_include_directories(HuffmanLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 2. Демонстраційна програма
add_executable(HuffmanDemo
    demo/main.cpp
)
target_link_libraries(HuffmanDemo PRIVATE HuffmanLib)

# 3. Демонстрація антипатернів
add_executable(AntipatternDemo
    antipattern/main.cpp
)
target_link_libraries(AntipatternDemo PRIVATE HuffmanLib)

# 4. Автоматичне завантаження Google Test через FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(HuffmanTests
    tests/HuffmanTests.cpp
)
target_link_libraries(HuffmanTests PRIVATE
    HuffmanLib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(HuffmanTests)