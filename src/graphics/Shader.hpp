#pragma once
#include <iostream>
#include <glm/glm.hpp>
namespace Graphics
{
    class Shader
    {
    private:
        unsigned int shaderProgram;
        std::string vertexShaderPath;
        std::string fragmentShaderPath;
    public:
        Shader(std::string vertexShaderPath,std::string fragmentShaderPath);
        ~Shader();
        void compileShader();
        void bindShader();
        void setUniformf(const std::string &name, float value);
        void setUniformVec3(const std::string &name, glm::vec3 vec3);
        void setUniformMat4f(const std::string &name, glm::mat4 matrix);
    };    
} // namespace Graphics    

