#include "Projectile.hpp"

long nextprojid = 0;

Projectile::Projectile(glm::vec2 pos, float dir, float speed, int penetration)
{
    this->id = nextprojid++;
    this->pos = pos;
    this->dir = dir;
    this->speed = speed;
    this->penetration = penetration;
    this->isExisting = true;
}

Projectile::~Projectile()
{

}

bool Projectile::exists()
{
    return this->isExisting;
}

void Projectile::init()
{
    ResourceManager::loadTexture("data/milk_shot.png", true, "dart");
}

void Projectile::update(float deltatime, std::vector<Bloon*> bloons)
{
    bool bloon_hit = false;
    Bloon* b;

    //advance
    this->pos += glm::vec2(cos(dir), sin(dir)) * deltatime * speed;

    //hit bloons (only one of them per frame and per projectile)
    for(int i = 0; i < bloons.size() && this->penetration > 0 && !bloon_hit; i++)
    {
        b = bloons[i];

        if(glm::length(this->pos - b->get_pos()) < b->get_size() && this->hitBloons.find(b->get_id()) == this->hitBloons.end())
        {
            b->pop();
            this->hitBloons.insert(b->get_id());
            bloon_hit = true;
            penetration--;
        }
    }

    if(penetration < 1 || this->pos.x > WIDTH+30 || this->pos.x < -30 || this->pos.y > HEIGHT+30 || this->pos.y < -30)
    {
        this->isExisting = false;
    }
}

void Projectile::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("dart");
    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), glm::vec2(spriteTex.Width, spriteTex.Height), this->dir*180/M_PI, glm::vec3(1.0f, 1.0f, 1.0f), true);
    //renderer->drawSprite(ResourceManager::getTexture("dart"), this->pos, glm::vec2(1.0f,1.0f), this->dir*180/M_PI, glm::vec3(1.0f, 1.0f, 1.0f));
}
