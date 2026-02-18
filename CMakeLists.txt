cmake_minimum_required(VERSION 3.28)

# Project name and version
project(Rasterizer VERSION 0.1 LANGUAGES C CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_CXX_CLANG_TIDY
#        clang-tidy;
#        -p=${CMAKE_BINARY_DIR};
#        --extra-arg=-std=c++20
#)
set(CMAKE_CXX_CLANG_FORMAT clang-format)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2
find_package(SDL2 REQUIRED)

# Find all code files
file(GLOB_RECURSE RASTERIZER_HEADERS "Code/include/*.hpp" "Code/include/*.h")
file(GLOB_RECURSE RASTERIZER_SOURCES "Code/src/*.cpp")

# Add files to compilation
add_executable(Rasterizer ${RASTERIZER_HEADERS} ${RASTERIZER_SOURCES})

# Add include directory
target_include_directories(Rasterizer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Code/include" "${SDL2_INCLUDE_DIRS}")

# Include and link dependencies
# SDL2
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
