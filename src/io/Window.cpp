#include "Window.hpp"
#include "Input.hpp"
#include <stdexcept>
#include <iostream>
void errorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}
void IO::Window::init()
{
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For macOS
#endif
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }

    glfwSetCursorPosCallback(window, IO::Input::mousePositionCallback);
    glfwSetKeyCallback(window, IO::Input::keyCallback);
    glfwSetMouseButtonCallback(window, IO::Input::mouseButtonsCallback);

    glfwSetWindowUserPointer(window,this);
    glfwSetFramebufferSizeCallback(window,[](GLFWwindow* window,int w,int h){
        auto window_ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if(window_ptr != nullptr)
            window_ptr->OnResize((Viewport){w,h});
    });

    glfwMakeContextCurrent(window);
    glfwSwapInterval(enableVsync ? 1 : 0);

    if (gl3wInit())
    {
        throw std::runtime_error("Failed to initialize GL3W.");
    }
    if (!gl3wIsSupported(3, 3))
    {
        throw std::runtime_error("OpenGL 3.3 not supported.");
    }

    OnCreate();
}
void IO::Window::loop()
{
    float lastFrame = glfwGetTime();
    float deltaTime = 0.0;
    float sec = 0.0;
    int frames = 0;
    while (!glfwWindowShouldClose(window))
    {
        OnFrame(deltaTime);
        glfwSwapBuffers(window);
        glfwPollEvents();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (sec >= 1)
        {
            std::string titleFPS = title + " FPS: " + std::to_string(frames);
            glfwSetWindowTitle(window, titleFPS.c_str());
            frames = 0;
            sec -= 1.0f;
        }
        frames++;
        sec += deltaTime;
    }
}
void IO::Window::destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
IO::Window::Window(int width, int height, std::string title, bool showFPS, bool enableVsync) : width(width), height(height), title(title), showFPS(showFPS), enableVsync(enableVsync)
{
}
void IO::Window::run()
{
    init();
    loop();
    destroy();
}