cmake_minimum_required(VERSION 3.20)

#names the project
project(RubbleSim VERSION 0.1.0 LANGUAGES CXX)

#include loads additional Cmake fucntionaliyty
#FetchContent is a cmake module for downloading and adding extermal libraries 
# FetchContent_Declare describes a dependency that CMake may
# download.
#
# glfw is the local name we give the dependency.
#
# GIT_REPOSITORY specifies where its source code is stored.
#
# GIT_TAG 3.4 tells CMake to use GLFW version 3.4. Specifying a
# version helps every developer build with the same library code.
#
# set assigns a CMake variable.
#
# OFF represents a disabled option.
#
# CACHE places the value in the CMake configuration cache.
#
# BOOL says the value is Boolean, meaning either ON or OFF.
#
# "" is an empty description required by this form of set.
#
# FORCE replaces any older cached value.
#
# We disable the GLFW documentation, tests, and examples because
# RubbleSim only needs the GLFW library.
#
# FetchContent_MakeAvailable downloads GLFW when necessary and
# makes its CMake target available to our project.
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(rubblesim 
    src/main.cpp
    src/app/Application.cpp
)

target_include_directories(rubblesim
    PRIVATE
        include 
        glfw
)

