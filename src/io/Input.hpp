#pragma once
#include <GLFW/glfw3.h>
namespace IO
{
	class Input{
		static bool keys[348];
		static bool buttons[8];
		static float m_x, m_y;
	public:
		static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonsCallback(GLFWwindow* window, int button, int action, int mods);
		static const bool isKeyDown(int key);
		static const bool isButtonDown(int button);
		static const float getMouseX();
		static const float getMouseY();
		
		static float getInputAxisForward();// returns value from axis W and S x ={-1;0;1}
		static float getInputAxisSideway();// returns value from axis A and D x ={-1;0;1}
	};
} // namespace IO

