#pragma once
#include <string>
#include "glm/glm.hpp"
#include <GL/glew.h>

class Shader
{
private:
    GLuint program;
    GLuint loadShaders(const char *vertex_path, const char *fragment_path);
    std::string readFile(const char *filePath);
public:
    Shader(const char *vertex_path, const char *fragment_path);
    ~Shader();
    void setInt(const char *name, GLuint value) const;
    void setFloat(const char *name, float value) const;
    void setMat4(const char *name, glm::mat4 value) const;
    void bind() const;
    void unbind() const;
};
