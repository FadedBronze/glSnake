#pragma once
#include <string>
#include <glm/glm.hpp>

struct ShaderSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind();
	void Unbind();

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4(const std::string& name, const glm::mat4 matrix);
	void SetUniform1i(const std::string& name, int v0);
private:
	unsigned int GetUniformLocation(const std::string& name);

    ShaderSource ParseShader(const std::string& path);

    unsigned int CompileShader(unsigned int type, const std::string& source);

	unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader);
};

