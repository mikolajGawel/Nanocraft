#include "Game.hpp"
#include <blocks/Block.hpp>
#include <geom/Quad.hpp>
#include <util/Time.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Nanocraft::OnCreate()
{
    Blocks::initBlocks();
    Geom::Quad::init();

    glm::mat4 view = player->getView();
    glm::mat4 model = glm::mat4(1.0f);

    uiShader.compileShader();
    blockShader.compileShader();
    blockShader.bindShader();

    blockShader.setUniformMat4f("uView", view);
    blockShader.setUniformMat4f("uModel", model);

    worldAtlas.loadTexture();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, worldAtlas.getTexureID());

    blockShader.setUniformf("uCellWidth", worldAtlas.getCellWidthRatio());
    blockShader.setUniformf("uCellHeight", worldAtlas.getCellHeightRatio());
    blockShader.setUniformf("fadeStart", Blocks::Chunk::CHUNK_SIZE*(renderDistance-1));

    world->loadChunks(player->position); 
}
void Nanocraft::OnFrame(float deltaTime)
{
    Time::DELTA_TIME = deltaTime;

    glClearColor(0.45f, 0.6f, 0.7f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    blockShader.bindShader();
    blockShader.setUniformMat4f("uView", player->getView());
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), gameViewport.getAspectRatio(), 0.1f, 10000.0f);
    blockShader.setUniformMat4f("uProjection", projection);
    blockShader.setUniformVec3("cameraPos", player->position);
   
    world->render();
    world->update(player->position);
    
    if (IO::Input::isButtonDown(GLFW_MOUSE_BUTTON_LEFT))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (IO::Input::isKeyDown(GLFW_KEY_ESCAPE))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    player->update(*world);
}
void Nanocraft::OnResize(Viewport viewport){
    glViewport(0,0,viewport.width,viewport.height);
    gameViewport = viewport;
}
Nanocraft::Nanocraft(int width, int height,uint8_t renderDistance, bool showFPS, bool enableVSync) : 
    IO::Window(width, height, "Nanocraft", showFPS, enableVSync),
    worldAtlas(Graphics::TextureAtlas("resources/terrain.png",16,16)),
    blockShader(Graphics::Shader("resources/shaders/block.v.glsl", "resources/shaders/block.f.glsl")),
    uiShader(Graphics::Shader("resources/shaders/uishader.v.glsl", "resources/shaders/uishader.f.glsl")),
    player(std::make_shared<Player>(glm::vec3(0, 20, 0), -90.0f, 0.0f, 70.0f)),
    renderDistance(renderDistance),
    gameViewport((Viewport){width,height})
{
    srand(time(NULL));
    int seed = rand()% 300 + rand()%1000;
    
    world = std::make_unique<World>(std::make_unique<Terrain>(seed,(Terrain::TerrainSettings){20.0f,0.02f,4}),renderDistance,floor((float)renderDistance*0.7f));
}