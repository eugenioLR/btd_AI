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
    std::ifstream ifs("data/config.json");
    Json::Value config_json;
    ifs >> config_json;
    int skin_type = config_json["skin_type"].asInt();

    std::string skin_name;
    switch(skin_type)
    {
        case 0:
        {
            skin_name = "dart.png";
            break;
        }
        case 1:
        {
            skin_name = "milk_shot.png";
            break;
        }
        default:
        {
            skin_name = "skin_monkey.png";
        }
    }
    ResourceManager::loadTexture(("data/projectiles/" + skin_name).c_str(), true, "dart");
}

void Projectile::update(float deltatime, std::vector<Bloon*> bloons, int* money)
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
            (*money)++;
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
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, this->dir*180/M_PI, color, true);
}
