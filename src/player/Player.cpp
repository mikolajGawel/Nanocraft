#include "Player.hpp"
#include <util/Math.hpp>
#include <util/Time.hpp>
#include <util/Raycast.hpp>
using namespace IO;

Player::Player(glm::vec3 position, float yaw, float pitch, float fov)
    : Graphics::Camera(position, yaw, pitch, fov)
{
    up = glm::vec3(0, 1, 0);
    lastCursorPos = glm::vec2(Input::getMouseX(), Input::getMouseY());

    glm::vec3 directions = Graphics::Camera::calculateDirections(yaw, pitch);
    front = glm::normalize(directions);
    movement_front = glm::normalize(glm::vec3(directions.x, 0.0f, directions.z));
}

void Player::resetMouseInput()
{
    lastCursorPos = glm::vec2(-1, -1);
}

void Player::rotation()
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
    if (pitch > 89.9f)
        pitch = 89.9f;
    if (pitch < -89.9f)
        pitch = -89.9f;

    glm::vec3 directions = Graphics::Camera::calculateDirections(yaw, pitch);
    front = glm::normalize(directions);
    movement_front = glm::normalize(glm::vec3(directions.x, 0.0f, directions.z));
}

#include <iostream>
void Player::movement()
{
    lastframe = position;
    float deltaTime = Time::DELTA_TIME;
    float accel = movement_speed;
    // Forward / Backward
    if (Input::getInputAxisForward() != 0)
        forwardAcceleration = Math::accelerate(accel, maxSpeed * Input::getInputAxisForward(), forwardAcceleration);
    else
        forwardAcceleration = Math::slowdown(accel, forwardAcceleration);

    // Left / Right
    if (Input::getInputAxisSideway() != 0)
        sidewayAcceleration = Math::accelerate(accel, maxSpeed * Input::getInputAxisSideway(), sidewayAcceleration);
    else
        sidewayAcceleration = Math::slowdown(accel, sidewayAcceleration);

    if (Input::isKeyDown(GLFW_KEY_SPACE))
        position.y += movement_speed * 0.5f * deltaTime;
    if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
        position.y -= movement_speed * 0.5f * deltaTime;

    // Apply acceleration to movement vector
    glm::vec3 right = glm::normalize(glm::cross(up, front));

    acceleration += movement_front * forwardAcceleration;
    acceleration += right * sidewayAcceleration;

    position += acceleration * deltaTime;
    acceleration = glm::vec3(0.0f);
}
static bool destroy_pressed = false;


void Player::action(World &world)
{
    if (destroy_pressed == false && Input::isButtonDown(GLFW_MOUSE_BUTTON_1))
    {
        auto ray = Raycast::raycast(front,position,5,world);
        if(ray){
            glm::ivec3 blockToDestroy = ray->blockPos;
            std::cout << "x: " << blockToDestroy.x <<" y: " << blockToDestroy.y << " z: " << blockToDestroy.z << std::endl;
            std::cout << "dir: " << Raycast::blockSideToString(ray->blockSide) << std::endl;
            world.setBlock(blockToDestroy,Blocks::BLOCK_AIR);
        } 
    }
    destroy_pressed = Input::isButtonDown(GLFW_MOUSE_BUTTON_1);
}
void Player::update(World &world)
{
    rotation();
    movement();
    action(world);
}
