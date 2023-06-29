#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"

void glClearError();

bool glLogError(const char* functionName);

#define ASSERT(x) if(!(x)) { std::cout << "Execution stopped at: " << __FILE__ << ":" << __LINE__ << std::endl; __debugbreak(); }
#define GL_CALL(x) glClearError(); x; ASSERT(glLogError(#x))

class Renderer {
public:
    void Draw(VertexBuffer& vb, IndexBuffer& ib, Shader& shader);
    void Draw3D(VertexBuffer& vb, unsigned int count, Shader& shader);
    void Clear();
private:
};