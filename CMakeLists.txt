cmake_minimum_required(VERSION 3.23)
project(Dushenkov3DRenderer LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/W4)
endif()

set(SFML_BUILD_AUDIO   OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC     OFF CACHE BOOL "" FORCE)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/CMakeLists.txt")
    message(FATAL_ERROR
        "SFML submodule not found!\n"
        "Run: git submodule update --init --recursive")
endif()

add_subdirectory(external/SFML)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/app/application.cpp
    src/app/random.cpp
    src/app/runtime.cpp
    src/app/view.cpp
    src/geometry/bounding_box.cpp
    src/geometry/matrix4x4.cpp
    src/geometry/point3d.cpp
    src/geometry/point4d.cpp
    src/geometry/triangle.cpp
    src/io/obj_reader.cpp
    src/world/camera.cpp
    src/world/color.cpp
    src/world/frame.cpp
    src/world/light.cpp
    src/world/mesh.cpp
    src/world/renderer.cpp
    src/world/screen.cpp
    src/world/world.cpp
    src/world/z_buffer.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
