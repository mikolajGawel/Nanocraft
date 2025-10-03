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

#include <glm/gtc/matrix_transform.hpp> 
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
    GLFWwindow *window = glfwCreateWindow(800, 600, "Nanocraft: Morning_wood Edition", nullptr, nullptr);
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

    Blocks::Block block(glm::vec3(0,0,0),Blocks::BlockTextureMap::all(3));
    block.faces.east = false;
    Blocks::Block block2(glm::vec3(0,0,1),Blocks::BlockTextureMap::all(2));
    block2.faces.west = false;
    auto mesh = block.generateMesh();
    auto mesh2 = block2.generateMesh();
    for(size_t i : mesh2.indices){
        mesh.indices.push_back(i + mesh.vertices.size()/6);
    }
    mesh.vertices.insert(mesh.vertices.end(),mesh2.vertices.begin(),mesh2.vertices.end());

    
    std::vector<float> verticesVec = mesh.vertices;
    float* vertices = verticesVec.data();


    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesVec.size() * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 5));

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size()*sizeof(uint), mesh.indices.data(), GL_STATIC_DRAW);

    Graphics::Shader shader("resources/shaders/block.v.glsl","resources/shaders/block.f.glsl");
    shader.compileShader();
    shader.bindShader();
    
    IO::Freecam camera(glm::vec3(0,0,0),-90.0f,0.0f,70.0f);
    glm::mat4 projection = glm::perspective(glm::radians(70.0f),800.0f/600.0f,0.1f,100.0f);
    glm::mat4 view = camera.getView();
    glm::mat4 model = glm::mat4(1.0f);

    shader.setUniformMat4f("uProjection",projection);
    shader.setUniformMat4f("uView",view);
    shader.setUniformMat4f("uModel",model);

    Graphics::TextureAtlas textureAtlas("resources/terrain.png",16,16);
    textureAtlas.loadTexture();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureAtlas.getTexureID()); 
    shader.setUniformf("uCellWidth", textureAtlas.getCellWidthRatio());
    shader.setUniformf("uCellHeight",textureAtlas.getCellHeightRatio());
    // Main loop

    float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();

    float sec = 0.0f;
    int frames = 0;

    while (!glfwWindowShouldClose(window))
    {
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES,36*2,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        camera.update();
        shader.setUniformMat4f("uView",camera.getView());

        currentFrame = glfwGetTime();
        Time::DELTA_TIME = currentFrame - lastFrame;
        if(sec >= 1){
            frames = 0;
            sec = 0;
        }
        frames++;
        sec += Time::DELTA_TIME;
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
