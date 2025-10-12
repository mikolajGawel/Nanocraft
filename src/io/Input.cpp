#include "Input.hpp"

using namespace IO;
bool Input::keys[348];
bool Input::buttons[8];
float Input::m_x, Input::m_y;

void Input::mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	m_x = (float)xpos;
	m_y = (float)ypos;
}
void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keys[key] = (action != 0);
}
void Input::mouseButtonsCallback(GLFWwindow* window, int button, int action, int mods)
{
	buttons[button] = (action != 0);
}
const bool Input::isKeyDown(int key) { return keys[key]; }
const bool Input::isButtonDown(int button) { return buttons[button]; }
const float Input::getMouseX() { return m_x; }
const float Input::getMouseY() { return m_y; }
float Input::getInputAxisForward(){
    if (Input::isKeyDown(GLFW_KEY_W) && !Input::isKeyDown(GLFW_KEY_S)) return 1.0f;
    else if (!Input::isKeyDown(GLFW_KEY_W) && Input::isKeyDown(GLFW_KEY_S)) return -1.0f;
    return 0;
}

float Input::getInputAxisSideway(){
    if (Input::isKeyDown(GLFW_KEY_A) && !Input::isKeyDown(GLFW_KEY_D)) return 1.0f;
    else if (!Input::isKeyDown(GLFW_KEY_A) && Input::isKeyDown(GLFW_KEY_D)) return -1.0f;
    return 0;
}