cmake_minimum_required(VERSION 3.12)

project(Mandelbrot)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG -Wno-unused")
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wno-unused")

file(GLOB SRC src/*.c)
add_executable(mandelbrot ${SRC})

set(GLFW_BUILD_DOCS FALSE)
set(GLFW_BUILD_EXAMPLES FALSE)
set(GLFW_BUILD_TESTS FALSE)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)

target_link_libraries(mandelbrot
    glfw
    glad
)