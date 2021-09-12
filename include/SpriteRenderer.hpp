#pragma once

#include "Texture.hpp"
#include "Shader.hpp"

class SpriteRenderer
{
public:
    SpriteRenderer();
    SpriteRenderer(Shader shader);
    ~SpriteRenderer();

    void drawSprite(Texture2D, glm::vec2, glm::vec2, float, glm::vec3, bool);
private:
    Shader shader;
    unsigned int quadVAO;

    void initRenderData();
};
