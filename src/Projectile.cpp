#include "Projectile.hpp"

long nextprojid = 0;

Projectile::Projectile(glm::vec2 pos, DamageType dmg_type, float dir, float speed, int penetration, float lifetime)
{
    this->id = nextprojid++;
    this->dmg_type = dmg_type;
    this->pos = pos;
    this->dir = dir;
    this->speed = speed;
    this->penetration = penetration;
    this->lifetime = lifetime;
    this->isExisting = true;
}

bool Projectile::exists()
{
    return this->isExisting;
}

void Projectile::init()
{
    std::ifstream ifs("data/config.json");
    Json::Value config_json;
    ifs >> config_json;
    int skin_type = config_json["skin_type"].asInt();

    std::string dart_name;
    switch(skin_type)
    {
        case 0:
        {
            dart_name = "dart.png";
            break;
        }
        case 1:
        {
            dart_name = "milk_shot.png";
            break;
        }
        default:
        {
            dart_name = "dart.png";
        }
    }
    ResourceManager::loadTexture(("data/projectiles/" + dart_name).c_str(), true, "dart");
    ResourceManager::loadTexture("data/projectiles/bomb.png", true, "bomb");
    ResourceManager::loadTexture("data/projectiles/explosion.png", true, "explosion");
}

// Bomb
Bomb::Bomb(glm::vec2 pos, float dir, float speed, int penetration) : Projectile(pos, NO_DMG, dir, speed, penetration, 0.7)
{

}

void Bomb::update(float deltatime, std::vector<Bloon*>* bloons, std::vector<Projectile*>* proj, int* money)
{
    bool bloon_hit = false;
    Bloon* b;

    // decrement timer
    this->lifetime -= deltatime;

    //advance
    this->pos += glm::vec2(cos(dir), sin(dir)) * deltatime * speed;

    //hit bloons (only one of them per frame and per projectile)
    for(int i = 0; i < bloons->size() && this->penetration > 0 && !bloon_hit; i++)
    {
        b = bloons->at(i);
        bloon_hit = glm::length(this->pos - b->get_pos()) < b->get_size();
    }

    if(this->lifetime < 0 || bloon_hit){
        this->isExisting = false;
        proj->push_back(new Explosion(this->pos, 0, this->dir, this->penetration));
    }
}

void Bomb::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("bomb");
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, this->dir*180/M_PI, color, true);
}

// Explosion
Explosion::Explosion(glm::vec2 pos, float dir, float speed, int penetration) : Projectile(pos, EXPLOSION, dir, speed, penetration, 0.07)
{

}

void Explosion::update(float deltatime, std::vector<Bloon*>* bloons, std::vector<Projectile*>* proj, int* money)
{
    bool bloon_hit = false;
    Bloon* b;

    // decrement timer
    this->lifetime -= deltatime;

    // do damage on the last frame
    if(this->lifetime < 0)
    {
        this->isExisting = false;

        //hit all bloons in range
        for(int i = 0; i < bloons->size() && this->penetration > 0; i++)
        {
            b = bloons->at(i);

            if(glm::length(this->pos - b->get_pos()) < b->get_size() + 40)
            {
                b->pop(EXPLOSION);
                penetration--;
                (*money)++;
            }
        }
    }
}

void Explosion::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("explosion");
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, this->dir*180/M_PI, color, true);
}

// Dart
Dart::Dart(glm::vec2 pos, float dir, float speed, int penetration) : Projectile(pos, PIERCE, dir, speed, penetration, 0.7)
{

}

void Dart::update(float deltatime, std::vector<Bloon*>* bloons, std::vector<Projectile*>* proj, int* money)
{
    bool bloon_hit = false;
    Bloon* b;

    // decrement timer
    this->lifetime -= deltatime;

    //advance
    this->pos += glm::vec2(cos(dir), sin(dir)) * deltatime * speed;

    //hit bloons (only one of them per frame and per projectile)
    for(int i = 0; i < bloons->size() && this->penetration > 0 && !bloon_hit; i++)
    {
        b = bloons->at(i);

        if(glm::length(this->pos - b->get_pos()) < b->get_size() && this->hitBloons.find(b->get_id()) == this->hitBloons.end())
        {
            b->pop(PIERCE);
            this->hitBloons.insert(b->get_id());
            bloon_hit = true;
            penetration--;
            (*money)++;
        }
    }

    if(this->lifetime < 0 || penetration < 1 || this->pos.x > WIDTH+30 || this->pos.x < -30 || this->pos.y > HEIGHT+30 || this->pos.y < -30)
    {
        this->isExisting = false;
    }
}

void Dart::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("dart");
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, this->dir*180/M_PI, color, true);
}
