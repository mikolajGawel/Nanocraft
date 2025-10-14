#include "Game.hpp"
#include <blocks/Block.hpp>
#include <geom/Quad.hpp>
#include <util/Time.hpp>
#include <glm/gtc/matrix_transform.hpp>
void Nanocraft::OnCreate()
{
    Blocks::initBlocks();
    Geom::Quad::init();

    glm::mat4 view = camera->getView();
    glm::mat4 model = glm::mat4(1.0f);

    uiShader.compileShader();
    shader.compileShader();
    shader.bindShader();

    shader.setUniformMat4f("uView", view);
    shader.setUniformMat4f("uModel", model);

    worldAtlas.loadTexture();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, worldAtlas.getTexureID());

    shader.setUniformf("uCellWidth", worldAtlas.getCellWidthRatio());
    shader.setUniformf("uCellHeight", worldAtlas.getCellHeightRatio());

    world->loadChunks(camera->position);
}
void Nanocraft::OnFrame(float deltaTime)
{
    Time::DELTA_TIME = deltaTime;

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.bindShader();
    shader.setUniformMat4f("uView", camera->getView());
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
    shader.setUniformMat4f("uProjection", projection);
   
    world->render();
    world->update(camera->position);
    
    if (IO::Input::isButtonDown(GLFW_MOUSE_BUTTON_LEFT))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (IO::Input::isKeyDown(GLFW_KEY_ESCAPE))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    camera->update(*world);
}
Nanocraft::Nanocraft(int width, int height, bool showFPS, bool enableVSync) : IO::Window(width, height, "Nanocraft", showFPS, enableVSync)
{
}