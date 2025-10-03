#include "Freecam.hpp"
#include <util/Math.hpp>
#include <util/Time.hpp>

using namespace IO;


Freecam::Freecam(glm::vec3 position, float yaw, float pitch, float fov)
    : Graphics::Camera(position, yaw, pitch, fov)
{
    up = glm::vec3(0, 1, 0);
    lastCursorPos = glm::vec2(Input::getMouseX(), Input::getMouseY());

    glm::vec3 directions = Graphics::Camera::calculateDirections(yaw, pitch);
    front = glm::normalize(directions);
    movement_front = glm::normalize(glm::vec3(directions.x, 0.0f, directions.z));
}

void Freecam::resetMouseInput()
{
    lastCursorPos = glm::vec2(-1, -1);
}

void Freecam::rotation()
{
    if (lastCursorPos == glm::vec2(-1, -1))
    {
        lastCursorPos = glm::vec2(Input::getMouseX(), Input::getMouseY());
    }

    float offsetX = Input::getMouseX() - lastCursorPos.x;
    float offsetY = lastCursorPos.y - Input::getMouseY();

    lastCursorPos = glm::vec2(Input::getMouseX(), Input::getMouseY());

    offsetX *= sensitivity;
    offsetY *= sensitivity;

    yaw += offsetX;
    pitch += offsetY;

    // Clamp pitch to prevent camera flipping
    if (pitch > 89.9f) pitch = 89.9f;
    if (pitch < -89.9f) pitch = -89.9f;

    glm::vec3 directions = Graphics::Camera::calculateDirections(yaw, pitch);
    front = glm::normalize(directions);
    movement_front = glm::normalize(glm::vec3(directions.x, 0.0f, directions.z));
}

int getInputAxisForward(){
    if (Input::isKeyDown(GLFW_KEY_W) && !Input::isKeyDown(GLFW_KEY_S)) return 1;
    else if (!Input::isKeyDown(GLFW_KEY_W) && Input::isKeyDown(GLFW_KEY_S)) return -1;
    return 0;
}

int getInputAxisSideway(){
    if (Input::isKeyDown(GLFW_KEY_A) && !Input::isKeyDown(GLFW_KEY_D)) return 1;
    else if (!Input::isKeyDown(GLFW_KEY_A) && Input::isKeyDown(GLFW_KEY_D)) return -1;
    return 0;
}
void Freecam::movement()
{
    lastframe = position;
    float deltaTime = Time::DELTA_TIME;
    float accel = movement_speed * deltaTime;

    // Forward / Backward
    if(getInputAxisForward() != 0)
        forwardAcceleration = Math::accelerate(accel, maxSpeed * getInputAxisForward(), forwardAcceleration);
    else
        forwardAcceleration = Math::slowdown(accel, forwardAcceleration);

    // Left / Right
    if(getInputAxisSideway() != 0)
        sidewayAcceleration = Math::accelerate(accel, maxSpeed*getInputAxisSideway(), sidewayAcceleration);
    else
        sidewayAcceleration = Math::slowdown(accel, sidewayAcceleration);

    if(Input::isKeyDown(GLFW_KEY_SPACE))
        position.y += movement_speed * 0.5f * deltaTime;
    if(Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
        position.y -= movement_speed * 0.5 * deltaTime;

    // Apply acceleration to movement vector
    glm::vec3 right = glm::normalize(glm::cross(up, front));

    acceleration += movement_front * forwardAcceleration;
    acceleration += right * sidewayAcceleration;

    position += acceleration * deltaTime;
    acceleration = glm::vec3(0.0f);
}

void Freecam::update()
{
    rotation();
    movement();
}
