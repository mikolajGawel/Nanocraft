#pragma once
#include <io/Window.hpp>
#include <graphics/Shader.hpp>
#include <graphics/TextureAtlas.hpp>
#include <player/Player.hpp>
class Nanocraft: public IO::Window
{
    Graphics::Shader shader = Graphics::Shader("resources/shaders/block.v.glsl", "resources/shaders/block.f.glsl");
    Graphics::Shader uiShader = Graphics::Shader("resources/shaders/uishader.v.glsl", "resources/shaders/uishader.f.glsl");
    Graphics::TextureAtlas worldAtlas = Graphics::TextureAtlas("resources/terrain.png",16,16);
    std::shared_ptr<Player> camera = std::make_shared<Player>(glm::vec3(0, 20, 0), -90.0f, 0.0f, 70.0f);
    std::unique_ptr<World> world = std::make_unique<World>(20);
protected:
    virtual void OnCreate() override;
    virtual void OnFrame(float deltaTime) override;
public:
    Nanocraft(int width,int height,bool showFPS,bool enableVSync);
};

