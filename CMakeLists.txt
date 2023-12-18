cmake_minimum_required(VERSION 3.27)
project(gaming)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(gaming src/main.cpp
        src/kat/window.cpp
        src/kat/window.hpp
        src/kat/types.hpp
        src/kat/context.cpp
        src/kat/context.hpp)
target_include_directories(gaming PRIVATE src/)
target_link_libraries(gaming glfw Vulkan::Vulkan)
target_compile_definitions(gaming PRIVATE GLFW_INCLUDE_VULKAN)