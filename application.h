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

#include "camera.h"
#include "ocean.h"

#include <stb_image.h>

#include <GLFW/glfw3.h>

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

            void ProcessInput(GLFWwindow* window);

            inline GLFWwindow* GetWindow() { return window_; }

        private:
            GLFWwindow* window_;
            ImVec4 clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            int m_windowWidth = 1280;
            int m_windowHeight = 720;

            std::vector<float> vertices_positions_ = {};
            MyGL::Vao* vao_ = nullptr;
            MyGL::Program* program_ = nullptr;

            GLuint VAO, VBO, shaderProgram;

            Camera* camera_ = nullptr;
            float lastX = 0.0f;
            float lastY = 0.0f;

            float deltaTime = 0.0f;
            float lastFrame = 0.0f;

            bool firstMouse = true;
    };
}