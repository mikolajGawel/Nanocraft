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

    framebufferShader.compileShader();
    glGenFramebuffers(1,&framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

    glGenTextures(1,&framebuffer_tex);
    glBindTexture(GL_TEXTURE_2D,framebuffer_tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,gameViewport.width,gameViewport.height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,framebuffer_tex,0);

    glGenRenderbuffers(1,&rbo);
    glBindRenderbuffer(GL_RENDERBUFFER,rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,gameViewport.width,gameViewport.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "Error framebuffer\n";
    }

    glBindTexture(GL_TEXTURE_2D,0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

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
}
void Nanocraft::OnFrame(float deltaTime)
{

    Time::DELTA_TIME = deltaTime;
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    glClearColor(0.45f, 0.6f, 0.7f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D,worldAtlas.getTexureID()); 
    blockShader.bindShader();
    blockShader.setUniformMat4f("uView", player->getView());
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), gameViewport.getAspectRatio(), 0.1f, 10000.0f);
    blockShader.setUniformMat4f("uProjection", projection);
    blockShader.setUniformVec3("cameraPos", player->position);
   
    world->render();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.45f, 0.6f, 0.7f, 1.0f);
    glBindTexture(GL_TEXTURE_2D,framebuffer_tex);
    
    framebufferShader.bindShader();
    Geom::Quad::draw();

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

    glBindTexture(GL_TEXTURE_2D,framebuffer_tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,gameViewport.width,gameViewport.height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);

    glBindRenderbuffer(GL_RENDERBUFFER,rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,gameViewport.width,gameViewport.height);
}
Nanocraft::Nanocraft(int width, int height,uint8_t renderDistance, bool showFPS, bool enableVSync) : 
    IO::Window(width, height, "Nanocraft", showFPS, enableVSync),
    worldAtlas(Graphics::TextureAtlas("resources/terrain.png",16,16)),
    blockShader(Graphics::Shader("resources/shaders/block.v.glsl", "resources/shaders/block.f.glsl")),
    uiShader(Graphics::Shader("resources/shaders/uishader.v.glsl", "resources/shaders/uishader.f.glsl")),
    framebufferShader(Graphics::Shader("resources/shaders/framebuffer.v.glsl", "resources/shaders/framebuffer.f.glsl")),
    player(std::make_shared<Player>(glm::vec3(0, 20, 0), -90.0f, 0.0f, 70.0f)),
    renderDistance(renderDistance),
    gameViewport((Viewport){width,height})
{
    srand(time(NULL));
    int seed = rand();
    
    world = std::make_unique<World>(std::make_unique<Terrain>(seed,(Terrain::TerrainSettings){20.0f,0.02f,4}),renderDistance,floor((float)renderDistance*0.7f));
}