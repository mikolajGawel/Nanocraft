#pragma once
#include <io/Window.hpp>
#include <graphics/Shader.hpp>
#include <graphics/TextureAtlas.hpp>
#include <player/Player.hpp>
class Nanocraft: public IO::Window
{
    Graphics::Shader blockShader;
    Graphics::Shader uiShader;
    Graphics::Shader framebufferShader;
    Graphics::TextureAtlas worldAtlas;
    std::shared_ptr<Player> player;
    std::unique_ptr<World> world;
    uint8_t renderDistance;
    Viewport gameViewport;

    unsigned int framebuffer,rbo,framebuffer_tex;
protected:
    void OnResize(Viewport viewport) override;
    void OnCreate() override;
    void OnFrame(float deltaTime) override;
public:
    Nanocraft(int width,int height,uint8_t renderDistance,bool showFPS,bool enableVSync);
};

