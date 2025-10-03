#include "Shader.hpp"
#include <GL/gl3w.h>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
using namespace Graphics;
Shader::Shader(std::string vertexShaderPath,std::string fragmentShaderPath){
    this->vertexShaderPath = vertexShaderPath;
    this->fragmentShaderPath = fragmentShaderPath;
    shaderProgram = 0;
}
Shader::~Shader(){
    if(shaderProgram != 0)
        glDeleteProgram(shaderProgram);
}

uint createShader(int type,std::string path){
    std::ifstream shaderFile(path);
    if(!shaderFile){
        throw new std::runtime_error("No shaderfile found at:" + path);
    }
    std::stringstream ss;
    ss << shaderFile.rdbuf();
    auto shader_str = ss.str();
    auto shader_src = shader_str.c_str();

    int shader = glCreateShader(type);
    glShaderSource(shader,1,&shader_src,0);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&result);
    if(result == 0){
        int len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string errorLog(' ',len);
        glGetShaderInfoLog(shader, len, &len, &errorLog[0]);
        std::cerr << "Shader Error (" + path + "):" <<  errorLog << "\n";
        return 0;
    }
    return shader;
}


void Shader::compileShader(){
    uint vertex = createShader(GL_VERTEX_SHADER,vertexShaderPath);
    uint fragment = createShader(GL_FRAGMENT_SHADER,fragmentShaderPath);
if (vertex == 0 || fragment == 0) {
    throw std::runtime_error("Shader compilation failed.");
}

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertex);
    glAttachShader(shaderProgram,fragment);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    
    int result;
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&result);
    if(result == GL_FALSE){
        int len;
        glGetProgramiv(shaderProgram,GL_INFO_LOG_LENGTH,&len);
        std::string errorLog(' ',len);
        glGetProgramInfoLog(shaderProgram,len,&len,&errorLog[0]);
        std::cerr << "Shader Program Link Error (" + vertexShaderPath + "; " + fragmentShaderPath + "): " + errorLog;
    }


    glDetachShader(shaderProgram,vertex);
    glDetachShader(shaderProgram,fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Shader::bindShader(){
    glUseProgram(shaderProgram);
}
void Shader::setUniformf(const std::string &name, float value) {
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist or is not used in shader program.\n";
    } else {
        glUniform1f(location, value);
    }
}
void Shader::setUniformMat4f(const std::string &name, glm::mat4 matrix) {
    int location = glGetUniformLocation(shaderProgram, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist or is not used in shader program.\n";
    } else {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}