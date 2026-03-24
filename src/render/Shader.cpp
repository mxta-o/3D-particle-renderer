#include "render/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

using namespace std;

Shader::Shader(const string& vertexPath, const string& fragmentPath) {
    string vsrc = readFile(vertexPath);
    string fsrc = readFile(fragmentPath);

    unsigned int v = compile(GL_VERTEX_SHADER, vsrc.c_str());
    unsigned int f = compile(GL_FRAGMENT_SHADER, fsrc.c_str());

    ID_ = glCreateProgram();
    glAttachShader(ID_, v);
    glAttachShader(ID_, f);
    glLinkProgram(ID_);

    int success;
    glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(ID_, 512, NULL, info);
        cerr << "PROGRAM LINK ERROR: " << info << "\n";
    }

    glDeleteShader(v);
    glDeleteShader(f);
}

Shader::~Shader() {
    if (ID_) glDeleteProgram(ID_);
}

void Shader::use() {
    glUseProgram(ID_);
}

string Shader::readFile(const string& path) {
    ifstream in(path);
    if (!in) {
        cerr << "Failed to open shader: " << path << "\n";
        return string();
    }
    stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

unsigned int Shader::compile(unsigned int type, const char* source) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(id, 512, NULL, info);
        std::cerr << "SHADER COMPILE ERROR: " << info << "\n";
    }
    return id;
}
