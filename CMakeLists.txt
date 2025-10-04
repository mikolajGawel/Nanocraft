cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(NanoCraft VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)


add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/graphics/Shader.cpp
    src/graphics/Camera.cpp
    src/graphics/Texture.cpp
    src/blocks/Chunk.cpp
    src/blocks/Block.cpp
    src/geom/BlockFace.cpp
    src/geom/BlockMesh.cpp
    src/geom/ChunkMesh.cpp
    src/geom/Quad.cpp
    src/world/World.cpp
    src/io/Input.cpp
    src/io/Freecam.cpp
    src/util/Time.cpp
    src/util/Math.cpp
)


if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

# Include submodules
add_subdirectory(externals/glfw)
add_subdirectory(externals/gl3w)

# Find OpenGL (System Library)
if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    if(NOT OPENGL_LIBRARY)
        message(FATAL_ERROR "OpenGL framework not found!")
    endif()
    set(OPENGL_LIBRARIES "-framework OpenGL")
else()
    find_package(OpenGL REQUIRED)
 #  set(OPENGL_LIBRARIES OpenGL)
endif()

# Freetype paths (Use Freetype targets instead)
set(FREETYPE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/externals/freetype/include)

target_include_directories(${PROJECT_NAME} 
    PUBLIC externals/glfw/include
    PUBLIC externals/gl3w/include
    PUBLIC externals/glm/
    PUBLIC externals/stb
    PUBLIC src/
)

add_library(gl3w_lib externals/gl3w/src/gl3w.c)
target_include_directories(gl3w_lib 
    PUBLIC externals/gl3w/include
)

# Correctly Link Libraries
target_link_libraries(${PROJECT_NAME}
    glfw
    gl3w_lib
    ${OPENGL_LIBRARIES}
)

# Copy Resources
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
