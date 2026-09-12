cmake_minimum_required(VERSION 3.20)

#names the project
project(RubbleSim VERSION 0.1.0 LANGUAGES C CXX)

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

# GLAD's generator is a Python module. Prefer the Python interpreter available
# from the command line so its installed modules are used during the build.
find_program(RUBBLESIM_PYTHON_EXECUTABLE NAMES python3 REQUIRED)
set(Python_EXECUTABLE "${RUBBLESIM_PYTHON_EXECUTABLE}")

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.4
)

#SOURCE_SUBDIR cmake tells CMake that GLAD keeps its CMake project inside the cmake directory.
#glad_add_library generates an OpenGL loader.
#glad_gl_core_33 is our name for the generated library.
#STATIC places the GLAD implementation directly into our executable.
#REPRODUCIBLE ensures generation uses a fixed OpenGL specification.
#API gl:core=3.3 requests OpenGL 3.3 with the core profile.
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)

glad_add_library(
    glad_gl_core_33
    STATIC
    REPRODUCIBLE
    API gl:core=3.3
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glm 
    GIT_REPOSITORY
    https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(rubblesim 
    src/main.cpp
    src/app/Application.cpp
)


# target_include_directories porvides lcoation contain ing you header files
# target_include_libraries connects compiled libraries to your executable

target_include_directories(rubblesim
    PRIVATE
        include
)

target_link_libraries(rubblesim
    PRIVATE
        glfw
        glad_gl_core_33
        glm::glm
)
