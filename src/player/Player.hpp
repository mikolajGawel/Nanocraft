#pragma once
#include <math.h>
#include <glm/glm.hpp>
#include "io/Input.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "world/World.hpp"
#include "graphics/Camera.hpp"

class Player: public Graphics::Camera
{
private:
	float movement_speed = 10.0f;
	float maxSpeed = 8.0f;

	glm::vec2 lastCursorPos;
   	glm::vec3 movement_front = glm::vec3(0, 0, 0),
		acceleration = glm::vec3(0,0,0);
		
	//requierd for movement and calculating acceleration
	float forwardAcceleration = 0.0;
	float sidewayAcceleration = 0.0;

	float sensitivity = 0.1f;
	glm::vec3 lastframe = glm::vec3(0,0,0);
	
	void action(World& world);//destroy and place block function 
	void rotation();
	void movement();
public:
    Player(glm::vec3 position,float yaw,float pitch,float fov);
    void update(World& world);
    void resetMouseInput();
};

