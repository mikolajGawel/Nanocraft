#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "graphics/Shader.hpp"
#include "graphics/Camera.hpp"
#include "graphics/TextureAtlas.hpp"
#include "geom/Quad.hpp"
#include "blocks/Block.hpp"
#include "io/Input.hpp"
#include "util/Time.hpp"
#include "io/Freecam.hpp"
#include "blocks/Chunk.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "world/World.hpp"
void errorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int main()
{
    // Set error callback
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // Set OpenGL version (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For macOS
#endif

    // Create window
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Nanocraft: Morning_wood Edition", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, IO::Input::mousePositionCallback);
    glfwSetKeyCallback(window, IO::Input::keyCallback);
    glfwSetMouseButtonCallback(window, IO::Input::mouseButtonsCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Enable vsync

    // Initialize GL3W
    if (gl3wInit())
    {
        std::cerr << "Failed to initialize GL3W." << std::endl;
        return -1;
    }

    if (!gl3wIsSupported(3, 3))
    {
        std::cerr << "OpenGL 3.3 not supported." << std::endl;
        return -1;
    }
    Blocks::initBlocks();

    // Blocks::Chunk chunk = Blocks::Chunk::generateFlatChunk(glm::ivec2(0,0),16);
    // auto& chunkMesh = chunk.getChunkMesh();
    
    Graphics::Shader shader("resources/shaders/block.v.glsl", "resources/shaders/block.f.glsl");
    shader.compileShader();
    shader.bindShader();
    
    std::shared_ptr<IO::Freecam> camera = std::make_shared<IO::Freecam>(glm::vec3(0, 18, 0), -90.0f, 0.0f, 70.0f);
    std::unique_ptr<World> world = std::make_unique<World>(camera,10);

    glm::mat4 projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 10000.0f);
    glm::mat4 view = camera->getView();
    glm::mat4 model = glm::mat4(1.0f);

    shader.setUniformMat4f("uProjection", projection);
    shader.setUniformMat4f("uView", view);
    shader.setUniformMat4f("uModel", model);

    Graphics::TextureAtlas textureAtlas("resources/terrain.png", 16, 16);
    textureAtlas.loadTexture();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAtlas.getTexureID());
    shader.setUniformf("uCellWidth", textureAtlas.getCellWidthRatio());
    shader.setUniformf("uCellHeight", textureAtlas.getCellHeightRatio());

    // Main loop

    float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();

    float sec = 0.0f;
    int frames = 0;
    bool cooldown = true;
    bool firstRefresh = false;

    world->create(world->getChunkPosition(camera->position));
    while (!glfwWindowShouldClose(window))
    {
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bindShader();
        shader.setUniformMat4f("uView", camera->getView());

        world->render();
        world->update();
        if (IO::Input::isKeyDown(GLFW_KEY_F5) && cooldown)
        {
            cooldown = false;
            world->refresh();
        }
        if(IO::Input::isButtonDown(GLFW_MOUSE_BUTTON_LEFT)){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (IO::Input::isKeyDown(GLFW_KEY_ESCAPE))
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();

        camera->update();

        currentFrame = glfwGetTime();
        Time::DELTA_TIME = currentFrame - lastFrame;
        if (sec >= 1)
        {
            if(!firstRefresh ){
                firstRefresh = true;
                world->refresh();
            }
            std::string title = "Nanocraft FPS: " + std::to_string(frames);
            glfwSetWindowTitle(window, title.c_str());
            frames = 0;
            sec -= 1.0f;
            cooldown = true;
        }
        frames++;
        sec += Time::DELTA_TIME;
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
