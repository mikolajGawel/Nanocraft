#pragma once
#include <glm/glm.hpp>
namespace Graphics
{
    class Camera
    {
    protected:
        glm::vec3 up,front;
        float pitch,yaw;
    public:
        static glm::vec3 calculateDirections(float pitch,float yaw);
        float fov;
        glm::vec3 position;
        Camera(glm::vec3 position,float pitch,float yaw,float fov);
    
        glm::mat4 getView();
        float getPitch();
        float getYaw();
        glm::vec3 getFront();
    };
} // namespace Engine::Graphics

