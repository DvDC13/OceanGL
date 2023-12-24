#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#include "vao.h"
#include "program.h"

#include <stb_image.h>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace Application
{
    class Application
    {
        public:
            Application();
            ~Application();
            
            void Run();

            void Start();
            void Update();

            void LoadImage(const std::string& filename);

            void SaveImageAsPPM(const std::string& inputPath, const std::string& outputPath);

            inline GLFWwindow* GetWindow() { return window_; }

        private:
            GLFWwindow* window_;
            ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            std::vector<float> vertices_positions_ = {};
            MyGL::Vao* vao_ = nullptr;
            MyGL::Program* program_ = nullptr;

            GLuint VAO, VBO, shaderProgram;
    };
}