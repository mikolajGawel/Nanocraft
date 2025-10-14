#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
namespace IO
{
    class Window
    {
    private:
        void init();    
        void loop();
        void destroy();

        int width,height;
        std::string title;
        bool showFPS,enableVsync;
    protected:

        GLFWwindow* window;
        virtual void OnCreate() = 0;
        virtual void OnFrame(float deltaTime) = 0;

    public:
        Window(int width,int height,std::string title,bool showFPS,bool enableVsync);
        void run();
    };

} // namespace IO

