#ifndef GLSHADER_LIBRARY_H
#define GLSHADER_LIBRARY_H

#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    static void checkCompileErrors(GLuint shader, const std::string& type);
};

#endif //GLSHADER_LIBRARY_H