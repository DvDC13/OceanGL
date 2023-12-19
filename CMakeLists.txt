cmake_minimum_required(VERSION 3.22.1)
project(oceanGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(imgui/sources)
include_directories(imgui/backend)
include_directories(include/)

include_directories(stb_image)
add_library(stb_image STATIC stb_image/stb_image.cpp
                             stb_image/stb_image_write.cpp)

add_library(imgui_lib STATIC)
target_link_directories(imgui_lib PUBLIC imgui/sources imgui/backend)
add_subdirectory(imgui)

add_library(utils_lib STATIC)
target_link_directories(utils_lib PUBLIC include)
add_subdirectory(src)

add_executable(oceanGL main.cpp application.cpp)

target_link_libraries(oceanGL ${OPENGL_LIBRARIES} glfw GLEW::GLEW stb_image imgui_lib utils_lib)