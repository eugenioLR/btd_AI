#include "Bloon.hpp"

long nextbloonid = 0;

Bloon::Bloon()
{
    this->id = nextbloonid++;
    this->pos = glm::vec2(0.0,0.0);
    this->layers = 1;
    this->speed = 1;
    this->progress = 0;
    this->is_frozen = false;
    this->isExisting = true;
}

Bloon::Bloon(std::queue<glm::vec2> path, double size, int layers, double speed)
{
    this->id = nextbloonid++;
    this->pos = path.front();
    this->path = path;
    this->size = size;
    this->layers = layers;
    this->speed = speed;
    this->progress = 0;
    this->is_frozen = false;
    this->isExisting = true;
}

Bloon::~Bloon()
{

}

bool Bloon::exists()
{
    return this->isExisting;
}

void Bloon::init()
{
    ResourceManager::loadTexture("data/donut.png", true, "bloon");
}

long Bloon::get_id()
{
    return this->id;
}

double Bloon::get_size()
{
    return this->size;
}

glm::vec2 Bloon::get_pos()
{
    return this->pos;
}

double Bloon::get_progress()
{
    return this->progress;
}

void Bloon::pop()
{
    this->layers--;
}

void Bloon::update(float deltatime)
{
    glm::vec2 direction = path.front() - this->pos;
    glm::vec2 step = deltatime * speed * glm::normalize(direction);
    if(glm::length(direction) > glm::length(step)*1.25 && !this->path.empty())
    {
        this->progress = (100 - path.size()) * 10000 - glm::length(direction);
        this->pos += step;
    }
    else if(!this->path.empty())
    {
        this->pos = this->path.front();
        this->path.pop();
    }
    else
    {
        this->finished = true;
        this->isExisting = false;
    }

    if(layers < 1)
        this->isExisting = false;
}


void Bloon::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("bloon");

    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), glm::vec2(spriteTex.Width, spriteTex.Height), 0, glm::vec3(1.0f, 1.0f, 1.0f), true);
    //sRenderer->drawSprite(ResourceManager::getTexture("bloon"), this->pos, glm::vec2(1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));
}
