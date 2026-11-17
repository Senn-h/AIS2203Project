cmake_minimum_required(VERSION 3.21)
project(aisproject CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

find_package(Threads REQUIRED)

include(FetchContent)
set(THREEPP_BUILD_TESTS OFF)
set(THREEPP_BUILD_EXAMPLES OFF)
FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 2026-09-28
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(threepp)

add_executable(sim src/main.cpp)
target_link_libraries(sim PRIVATE threepp::threepp Threads::Threads)