#ifndef SCENE_H
#define SCENE_H
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>
#include "shader.h"

class scene
{
public:
    scene(int w, int h, glm::mat4 m, glm::mat4 v, glm::mat4 p);
    void draw();
    void init();
    void preCalculatePass();
    void updateMatrix(glm::mat4 m, glm::mat4 v, glm::mat4 p);
private:
    void initShaders();
    void initVAOs();
    void initGbufferFrame();

    //shaders
    std::shared_ptr<Shader> cubeGeoPass;
    std::shared_ptr<Shader> planeGeoPass;
    std::shared_ptr<Shader> backDepthRenderPass;
    std::shared_ptr<Shader> postProcessPass;

    //matrix
    glm::mat4 model{1.0f};
    glm::mat4 view{1.0f};
    glm::mat4 projection{1.0f};

    //screen config
    int width, height;

    //buffers
    unsigned int cubeVAO, cubeVBO;
    unsigned int planeVAO, planeVBO;
    unsigned int quadVAO, quadVBO;

    //gBuffers and textures
    unsigned int gBuffer;
    unsigned int gPosition, gNormal, gColor, gBackDepth, rbo;

    //datas
    //1.cube position:3 normal:3 texcoord:2
    std::vector<float> cubeVertices = {
    // positions          // normals           // texture coords
    // Back face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left 
    };

    //2.plane position:3 normal:3 texcoord:2
    std::vector<float> planeVertices = {
        5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

         5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };

    //3.quad position:3 texcoord:2
    std::vector<float> quadVertices = {
        // positions        // texture coords
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top right
         -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  // top left 
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right

         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  // top left 
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f // bottom left
    };
};
#endif