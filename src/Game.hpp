#pragma once
#include <io/Window.hpp>
#include <graphics/Shader.hpp>
#include <graphics/TextureAtlas.hpp>
#include <player/Player.hpp>
class Nanocraft: public IO::Window
{
    Graphics::Shader blockShader;
    Graphics::Shader uiShader;
    Graphics::TextureAtlas worldAtlas;
    std::shared_ptr<Player> player;
    std::unique_ptr<World> world;
protected:
    virtual void OnCreate() override;
    virtual void OnFrame(float deltaTime) override;
public:
    Nanocraft(int width,int height,bool showFPS,bool enableVSync);
};

