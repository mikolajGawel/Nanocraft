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
        struct Viewport{
            int width,height;
            inline float getAspectRatio(){
                if(height == 0)return 0;
                return (float)width/(float)height;
            }
        };
        GLFWwindow* window;
        virtual void OnCreate() = 0;
        virtual void OnFrame(float deltaTime) = 0;
        virtual void OnResize(Viewport viewport) {}
    public:
        Window(int width,int height,std::string title,bool showFPS,bool enableVsync);
        void run();
    };

} // namespace IO

