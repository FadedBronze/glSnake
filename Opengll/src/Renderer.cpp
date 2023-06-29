#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>


void glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool glLogError(const char* functionName) {
    bool noError = true;

    while (GLenum err = glGetError()) {
        std::cout << "[Opengl Error](code: " << err << ")" << std::endl;
        std::cout << "at: " << functionName << std::endl;
        noError = false;
    }

    return noError;
}

void Renderer::Draw(VertexBuffer& vb, IndexBuffer& ib, Shader& shader)
{
    vb.Bind();
    ib.Bind();
    shader.Bind();
    GL_CALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw3D(VertexBuffer& vb, unsigned int count, Shader& shader)
{
    vb.Bind();
    shader.Bind();
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, count));
}

void Renderer::Clear()
{
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}
