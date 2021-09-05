#include "scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

scene::scene(int w, int h, glm::mat4 m, glm::mat4 v, glm::mat4 p)
{
    width = w;
    height = h;
    updateMatrix(m, v, p);
    init();
}

void scene::draw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColor);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gBackDepth);

    postProcessPass->use();
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void scene::init()
{
    initShaders();
    initVAOs();
    initGbufferFrame();
}

void scene::updateMatrix(glm::mat4 m, glm::mat4 v, glm::mat4 p)
{
    model = m;
    view = v;
    projection = p;
}

void scene::initShaders()
{
    cubeGeoPass = std::make_shared<Shader>("../shaders/cubeGeo.vs", "../shaders/cubeGeo.fs");
    planeGeoPass = std::make_shared<Shader> ("../shaders/planeGeo.vs", "../shaders/planeGeo.fs");
    backDepthRenderPass = std::make_shared<Shader>("../shaders/backDepthRender.vs", "../shaders/backDepthRender.fs");
    postProcessPass = std::make_shared<Shader>("../shaders/postProcess.vs", "../shaders/postProcessWithBinarySearch.fs");
    postProcessPass->use();
    postProcessPass->setInt("gPosition", 0);
    postProcessPass->setInt("gNormal", 1);
    postProcessPass->setInt("gColor", 2);
    postProcessPass->setInt("gBackDepth", 3);
    postProcessPass->setMat4("projection", projection);
}

void scene::initVAOs()
{
    //cube VAO
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeVertices.size(), &cubeVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    //plane VAO
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planeVertices.size(), &planeVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    //quad VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quadVertices.size(), &quadVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
}

void scene::initGbufferFrame()
{
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    //1.position texture
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    //2.normal texture
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    //3.color texture
    glGenTextures(1, &gColor);
    glBindTexture(GL_TEXTURE_2D, gColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColor, 0);

    //3.1.back render depth
    glGenTextures(1, &gBackDepth);
    glBindTexture(GL_TEXTURE_2D, gBackDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBackDepth, 0);


    //4.tell opengl which color attachment we'll use for rendering
    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);

    //5.we also need a depth buffer(renderbuffer)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //6.finally check if framebuffer complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
}

void scene::preCalculatePass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);

    cubeGeoPass->use();
    cubeGeoPass->setMat4("view", view);
    cubeGeoPass->setMat4("projection", projection);
    backDepthRenderPass->use();
    backDepthRenderPass->setMat4("view", view);
    backDepthRenderPass->setMat4("projection", projection);
    planeGeoPass->use();
    planeGeoPass->setMat4("view", view);
    planeGeoPass->setMat4("projection", projection);

    //render cubes
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(cubeVAO);
    model = glm::translate(model, {-1.0f, 0.8f, -0.5f});
    cubeGeoPass->use();
    cubeGeoPass->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    auto preModel = model;
    model = glm::mat4(1.0f);
    model = glm::translate(model, {1.0f, 0.5f, -0.5f});
    cubeGeoPass->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //render back cubes
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    backDepthRenderPass->use();
    backDepthRenderPass->setMat4("model", preModel);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    backDepthRenderPass->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //render floor
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(planeVAO);
    model = glm::mat4(1.0f);
    planeGeoPass->use();
    planeGeoPass->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //back render floor
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    backDepthRenderPass->use();
    backDepthRenderPass->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_CULL_FACE);
}