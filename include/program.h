#pragma once

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "error.h"

namespace MyGL
{
    class Program
    {
    public:
        struct Shaders
        {
            std::string vertex_shader_path;
            std::string fragment_shader_path;
            std::string tessellation_control_shader_path;
            std::string tessellation_evaluation_shader_path;
        };

    public:
        Program(); 

        static Program* make_program(std::string& vertex_shader_path, std::string& fragment_shader_path,
                std::string& tessellation_control_shader_path, std::string& tessellation_evaluation_shader_path);

        static Program* make_program(std::string& vertex_shader_path, std::string& fragment_shader_path);
        
        char* get_log();
        bool is_ready();
        void use();
        void unuse();

        inline unsigned int get_program_id() const { return m_ProgramID; }

        void delete_program();

        int getUniformLocation(const std::string& name);

        void setUniform1f(const std::string& name, float v0);
        void setUniform1i(const std::string& name, int v0);
        void setUniformVec3f(const std::string& name, float v0, float v1, float v2);
        void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

    private:
        Shaders storeShaders(std::string& vertex_shader_path, std::string& fragment_shader_path);
        Shaders storeShaders(std::string& vertex_shader_path, std::string& fragment_shader_path,
                std::string& tessellation_control_shader_path, std::string& tessellation_evaluation_shader_path);

        bool is_shader_ready(unsigned int shader_id);
        char* get_shader_log(unsigned int shader_id);
        unsigned int compileShader(unsigned int type, const std::string& source);

    private:
        unsigned int m_ProgramID;
        std::unordered_map<std::string, int> m_UniformLocationCache;
    };
}