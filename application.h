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

#include <stb_image.h>

#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

inline std::vector<std::string> skybox_faces =
{
    "../skybox_images/right.jpg",
    "../skybox_images/left.jpg",
    "../skybox_images/top.jpg",
    "../skybox_images/bottom.jpg",
    "../skybox_images/front.jpg",
    "../skybox_images/back.jpg"
};

inline unsigned int loadCubemapTexture(std::vector<std::string> faces)
{
    stbi_set_flip_vertically_on_load(false);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
            {
                format = GL_RED;
            }
            else if (nrChannels == 3)
            {
                format = GL_RGB;
            }
            else if (nrChannels == 4)
            {
                format = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Failed to load cubemap texture" << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
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

            float time = 0.0f;
            int numberOfWaves;

            float roughness;
            float metallic;

            float amplitude;
            float frequency;
            float amplitude_attenuation;
            float frequency_amplification;

            glm::vec3 lightPos;
            glm::vec3 lightColor;
            float lightIntensity;

            glm::vec3 oceanColor;

            std::vector<float> skybox_vertices_ = {};
            MyGL::Vao* skybox_vao_ = nullptr;
            MyGL::Program* skybox_program_ = nullptr;
            unsigned int skybox_texture_;
    };
}