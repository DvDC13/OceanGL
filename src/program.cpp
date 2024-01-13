#include "program.h"

namespace MyGL
{
    Program::Program()
    {
        m_ProgramID = glCreateProgram(); CHECK_GL_ERROR();
    }

    Program* Program::make_program(std::string& vertex_shader_path, std::string& fragment_shader_path,
            std::string& tessellation_control_shader_path, std::string& tessellation_evaluation_shader_path)
    {
        Program* program = new Program();

        Shaders shaders = program->storeShaders(vertex_shader_path, fragment_shader_path, tessellation_control_shader_path, tessellation_evaluation_shader_path);
        unsigned int vertex_shader = program->compileShader(GL_VERTEX_SHADER, shaders.vertex_shader_path);
        unsigned int tessellation_control_shader = program->compileShader(GL_TESS_CONTROL_SHADER, shaders.tessellation_control_shader_path);
        unsigned int tessellation_evaluation_shader = program->compileShader(GL_TESS_EVALUATION_SHADER, shaders.tessellation_evaluation_shader_path);
        unsigned int fragment_shader = program->compileShader(GL_FRAGMENT_SHADER, shaders.fragment_shader_path);

        glAttachShader(program->m_ProgramID, vertex_shader); CHECK_GL_ERROR();
        glAttachShader(program->m_ProgramID, tessellation_control_shader); CHECK_GL_ERROR();
        glAttachShader(program->m_ProgramID, tessellation_evaluation_shader); CHECK_GL_ERROR();
        glAttachShader(program->m_ProgramID, fragment_shader); CHECK_GL_ERROR();

        glLinkProgram(program->m_ProgramID); CHECK_GL_ERROR();

        if (!program->is_ready())
        {
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED" << std::endl;
            char* log = program->get_log();
            std::cerr << log << std::endl;
            delete[] log;
            delete program;
            return nullptr;
        }

        glDeleteShader(vertex_shader); CHECK_GL_ERROR();
        glDeleteShader(tessellation_control_shader); CHECK_GL_ERROR();
        glDeleteShader(tessellation_evaluation_shader); CHECK_GL_ERROR();
        glDeleteShader(fragment_shader); CHECK_GL_ERROR();

        return program;
    }

    Program* Program::make_program(std::string& vertex_shader_path, std::string& fragment_shader_path)
    {
        Program* program = new Program();

        Shaders shaders = program->storeShaders(vertex_shader_path, fragment_shader_path);
        unsigned int vertex_shader = program->compileShader(GL_VERTEX_SHADER, shaders.vertex_shader_path);
        unsigned int fragment_shader = program->compileShader(GL_FRAGMENT_SHADER, shaders.fragment_shader_path);

        glAttachShader(program->m_ProgramID, vertex_shader); CHECK_GL_ERROR();
        glAttachShader(program->m_ProgramID, fragment_shader); CHECK_GL_ERROR();

        glLinkProgram(program->m_ProgramID); CHECK_GL_ERROR();

        if (!program->is_ready())
        {
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED" << std::endl;
            char* log = program->get_log();
            std::cerr << log << std::endl;
            delete[] log;
            delete program;
            return nullptr;
        }

        glDeleteShader(vertex_shader); CHECK_GL_ERROR();
        glDeleteShader(fragment_shader); CHECK_GL_ERROR();

        return program;
    }

    char* Program::get_log()
    {
        int length;
        glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &length); CHECK_GL_ERROR(); 
        char* log = new char[length];
        glGetProgramInfoLog(m_ProgramID, length, &length, log); CHECK_GL_ERROR();
        return log;
    }

    bool Program::is_ready()
    {
        int status;
        glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &status); CHECK_GL_ERROR();
        return status == GL_TRUE;
    }

    void Program::use()
    {
        glUseProgram(m_ProgramID); CHECK_GL_ERROR();
    }

    void Program::unuse()
    {
        glUseProgram(0); CHECK_GL_ERROR();
    }

    Program::Shaders Program::storeShaders(std::string& vertex_shader_path, std::string& fragment_shader_path)
    {
        std::string vertex_shader_code;
        std::string fragment_shader_code;
        std::ifstream vertex_shader_file;
        std::ifstream fragment_shader_file;

        vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertex_shader_file.open(vertex_shader_path);
            fragment_shader_file.open(fragment_shader_path);

            std::stringstream vertex_shader_stream, fragment_shader_stream;

            vertex_shader_stream << vertex_shader_file.rdbuf();
            fragment_shader_stream << fragment_shader_file.rdbuf();

            vertex_shader_file.close();
            fragment_shader_file.close();

            vertex_shader_code = vertex_shader_stream.str();
            fragment_shader_code = fragment_shader_stream.str();
        }
        catch (const std::exception& e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        return { vertex_shader_code, fragment_shader_code };
    }

    Program::Shaders Program::storeShaders(std::string& vertex_shader_path, std::string& fragment_shader_path,
            std::string& tessellation_control_shader_path, std::string& tessellation_evaluation_shader_path)
    {
        std::string vertex_shader_code;
        std::string fragment_shader_code;
        std::string tessellation_control_shader_code;
        std::string tessellation_evaluation_shader_code;
        std::ifstream vertex_shader_file;
        std::ifstream fragment_shader_file;
        std::ifstream tessellation_control_shader_file;
        std::ifstream tessellation_evaluation_shader_file;

        vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        tessellation_control_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        tessellation_evaluation_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertex_shader_file.open(vertex_shader_path);
            fragment_shader_file.open(fragment_shader_path);
            tessellation_control_shader_file.open(tessellation_control_shader_path);
            tessellation_evaluation_shader_file.open(tessellation_evaluation_shader_path);

            std::stringstream vertex_shader_stream, fragment_shader_stream, tessellation_control_shader_stream, tessellation_evaluation_shader_stream;

            vertex_shader_stream << vertex_shader_file.rdbuf();
            fragment_shader_stream << fragment_shader_file.rdbuf();
            tessellation_control_shader_stream << tessellation_control_shader_file.rdbuf();
            tessellation_evaluation_shader_stream << tessellation_evaluation_shader_file.rdbuf();

            vertex_shader_file.close();
            fragment_shader_file.close();
            tessellation_control_shader_file.close();
            tessellation_evaluation_shader_file.close();

            vertex_shader_code = vertex_shader_stream.str();
            fragment_shader_code = fragment_shader_stream.str();
            tessellation_control_shader_code = tessellation_control_shader_stream.str();
            tessellation_evaluation_shader_code = tessellation_evaluation_shader_stream.str();
        }
        catch (const std::exception& e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        return { vertex_shader_code, fragment_shader_code, tessellation_control_shader_code, tessellation_evaluation_shader_code };
    }

    bool Program::is_shader_ready(unsigned int shader_id)
    {
        int status;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status); CHECK_GL_ERROR();
        return status == GL_TRUE;
    }

    char* Program::get_shader_log(unsigned int shader_id)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length); CHECK_GL_ERROR();
        char* log = new char[length];
        glGetShaderInfoLog(shader_id, length, &length, log); CHECK_GL_ERROR();
        return log;
    }

    unsigned int Program::compileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type); CHECK_GL_ERROR();
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr); CHECK_GL_ERROR();
        glCompileShader(id); CHECK_GL_ERROR();

        if (!is_shader_ready(id))
        {
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED" << std::endl;

            std::cerr << "Shader type: ";
            if (type == GL_VERTEX_SHADER)
                std::cerr << "Vertex shader" << std::endl;
            else if (type == GL_TESS_CONTROL_SHADER)
                std::cerr << "Tessellation control shader" << std::endl;
            else if (type == GL_TESS_EVALUATION_SHADER)
                std::cerr << "Tessellation evaluation shader" << std::endl;
            else if (type == GL_FRAGMENT_SHADER)
                std::cerr << "Fragment shader" << std::endl;
            char* log = get_shader_log(id);
            std::cerr << log << std::endl;
            glDeleteShader(id); CHECK_GL_ERROR();
            delete log;
            return EXIT_FAILURE;
        }

        return id;
    }

    int Program::getUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
           return m_UniformLocationCache[name];
        
        int location = glGetUniformLocation(m_ProgramID, name.c_str()); CHECK_GL_ERROR();
        if (location == -1)
            std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

        m_UniformLocationCache[name] = location;
        return location;
    }

    void Program::setUniform1f(const std::string& name, float v0)
    {
        glUniform1f(getUniformLocation(name), v0); CHECK_GL_ERROR();
    }

    void Program::setUniform1i(const std::string& name, int v0)
    {
        glUniform1i(getUniformLocation(name), v0); CHECK_GL_ERROR();
    }

    void Program::setUniformVec3f(const std::string& name, float v0, float v1, float v2)
    {
        glUniform3f(getUniformLocation(name), v0, v1, v2); CHECK_GL_ERROR();
    }

    void Program::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)); CHECK_GL_ERROR();
    }

    void Program::delete_program()
    {
        glDeleteProgram(m_ProgramID); CHECK_GL_ERROR();
    }
}