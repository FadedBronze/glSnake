#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
    :m_RendererID(0)
{
    ShaderSource shaderSource = ParseShader("res/shaders/Simple.shader");
    m_RendererID = CreateShaders(shaderSource.vertexSource, shaderSource.fragmentSource);
    GL_CALL(glUseProgram(m_RendererID));
}

Shader::~Shader()
{
    GL_CALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind()
{
    GL_CALL(glUseProgram(m_RendererID));
}

void Shader::Unbind()
{
    GL_CALL(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GL_CALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4 matrix)
{
    GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
    GL_CALL(glUniform1i(GetUniformLocation(name), v0));
}


//add caching if u want later
unsigned int Shader::GetUniformLocation(const std::string& name)
{
    unsigned int uniformLocation = glGetUniformLocation(m_RendererID, name.c_str());
	return uniformLocation;
}

ShaderSource Shader::ParseShader(const std::string& path) {
    enum struct ShaderType {
        VERTEX = 0,
        FRAGMENT = 1,
        NONE = -1
    };

    std::ifstream stream(path);
    std::string line;
    std::stringstream ss[2];

    ShaderType shaderType = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("fragment") != std::string::npos) {
                shaderType = ShaderType::FRAGMENT;
            }
            if (line.find("vertex") != std::string::npos) {
                shaderType = ShaderType::VERTEX;
            }
        }
        else {
            if (shaderType == ShaderType::NONE) {
                std::cout << "shader type not included or shader doesn't exist" << std::endl;
                std::cout << line << std::endl;
            }

            ss[(int)shaderType] << line << '\n';
        }
    }

    return ShaderSource{
        ss[0].str(),
        ss[1].str()
    };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type); //generating the shaders id
    const char* src = source.c_str(); //getting a pointer of the starting character in the string (aka a c string)
    GL_CALL(glShaderSource(id, 1, &src, nullptr)); //pass in our src
    GL_CALL(glCompileShader(id)); //compiles our code

    int result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        int error_length;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &error_length));

        char* message = (char*)alloca(error_length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(id, error_length, &error_length, message));

        std::cout << message << std::endl;
        std::cout <<
            (
                type == GL_VERTEX_SHADER ? "VERTEX SHADER" :
                type == GL_FRAGMENT_SHADER ? "FRAGMENT SHADER" :
                "OTHER SHADER"
                ) << " Failed to compile." << std::endl;

        GL_CALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShaders(const std::string& vertexShader, const std::string& fragmentShader) {
    GL_CALL(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GL_CALL(glAttachShader(program, vs));
    GL_CALL(glAttachShader(program, fs));
    GL_CALL(glLinkProgram(program));
    GL_CALL(glValidateProgram(program));

    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));

    return program;
}