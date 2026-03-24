#pragma once

#include <string>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    void use();
    void setMat4(const std::string& name, const float* value);
private:
    unsigned int ID_ = 0;
    std::string readFile(const std::string& path);
    unsigned int compile(unsigned int type, const char* source);
};
