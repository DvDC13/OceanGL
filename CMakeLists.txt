cmake_minimum_required(VERSION 3.22.1)
project(oceanGL LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

FetchContent_Declare(glm
        URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip)

FetchContent_MakeAvailable(glm)

include_directories(imgui/sources)
include_directories(imgui/backend)
include_directories(include/)
include_directories(${FFTW3_SOURCE_DIR}/api)

include_directories(stb_image)
add_library(stb_image STATIC stb_image/stb_image.cpp
                             stb_image/stb_image_write.cpp)

add_library(imgui_lib STATIC)
target_link_libraries(imgui_lib stb_image glfw GLEW::GLEW) 
add_subdirectory(imgui)

add_library(utils_lib STATIC)
target_link_libraries(utils_lib imgui_lib stb_image glfw GLEW::GLEW)
add_subdirectory(src)

add_executable(oceanGL main.cpp application.cpp)

target_link_libraries(oceanGL ${OPENGL_LIBRARIES} glfw GLEW::GLEW glm stb_image imgui_lib utils_lib dl)