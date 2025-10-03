#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
using namespace Graphics;
glm::vec3 Camera::calculateDirections(float yaw,float pitch)
{
	glm::vec3 directions = glm::vec3(1.0f);
	directions.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	directions.y = sin(glm::radians(pitch));
	directions.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	return directions;
}
Camera::Camera(glm::vec3 position,float yaw,float pitch,float fov)
    :position(position),yaw(yaw),pitch(pitch),fov(fov){
    front = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	front = glm::normalize(Camera::calculateDirections( pitch,yaw));
}
glm::mat4 Camera::getView()
{
    return glm::lookAt(position, position + front,up);
}
float Camera::getPitch(){return pitch;}
float Camera::getYaw(){return yaw;}
