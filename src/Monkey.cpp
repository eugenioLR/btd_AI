#include "Monkey.hpp"

long nextmonkeyid = 0;

//General Tower
Monkey::Monkey(glm::vec2 pos)
{
    this->id = nextmonkeyid++;
    this->pos = pos;
    this->timer = 0;
    this->update1 = 0;
    this->update2 = 0;
    this->dir = 3*M_PI/2;
    this->state = COOLDOWN;
    this->isExisting = true;
}

Monkey::Monkey(glm::vec2 pos, float cooldown, int range)
{
    this->id = nextmonkeyid++;
    this->pos = pos;
    this->cooldown = cooldown;
    this->timer = 0;
    this->range = range;
    this->update1 = 0;
    this->update2 = 0;
    this->dir = 3*M_PI/2;
    this->state = COOLDOWN;
    this->isExisting = true;
}

int Monkey::get_cost()
{
    return this->cost;
}

bool Monkey::exists()
{
    return this->isExisting;
}

void Monkey::init()
{
    ResourceManager::loadTexture("data/towers/dart_monkey/skin.png", true, "dart_monkey");
    ResourceManager::loadTexture("data/towers/tack_shooter/skin.png", true, "tack_shooter");
}

void Monkey::update(float deltatime, std::vector<Bloon*> bloons, std::vector<Projectile*>* projectiles)
{
    Bloon* b = nullptr;
    Bloon* target = nullptr;

    glm::vec2 m_to_b;

    float max_progress = -1, dist;

    switch (state)
    {
        case CAN_SHOOT:
        {
            //target bloon
            for(int i = 0; i < bloons.size(); i++)
            {
                b = bloons[i];
                dist = glm::length(this->pos - b->get_pos());
                if(dist < this->range && max_progress < b->get_progress())
                {
                    max_progress = b->get_progress();
                    target = b;
                }
            }

            if(target != nullptr)
            {
                //look at bloon
                m_to_b = target->get_pos() - this->pos;
                this->dir = atan2(m_to_b.y, m_to_b.x);
            }

            if(target != nullptr)
            {
                //shoot at bloon
                shoot(target->get_pos(), projectiles);
                state = COOLDOWN;
                timer = 0;
            }
            break;
        }
        case COOLDOWN:
        {
            //update cooldown
            if(timer >= cooldown)
                state = CAN_SHOOT;

            break;
        }
    }

    if(timer <= cooldown)
        timer += deltatime;
}

//Dart Tower
DartMonkey::DartMonkey(glm::vec2 pos, float cooldown, int range) : Monkey(pos, cooldown, range)
{
    this->cost = 250;
    this->sell_value = 200;
}

DartMonkey::DartMonkey(glm::vec2 pos) : Monkey(pos)
{
    std::ifstream ifs("data/towers/dart_monkey/stats.json");
    Json::Value stats_json;
    ifs >> stats_json;
    this->cost = stats_json["cost"].asInt();
    this->sell_value = stats_json["sell_value"].asInt();
    this->cooldown = stats_json["cooldown"].asFloat();
    this->range = stats_json["range"].asInt();
    this->penetration = stats_json["penetration"].asInt();
    this->size = stats_json["size"].asInt();
}

void DartMonkey::shoot(glm::vec2 bloon_pos, std::vector<Projectile*>* projectiles)
{
    glm::vec2 newPosition = this->pos;
    glm::vec2 offset = glm::vec2(20, 20);
    glm::mat2 rotation = glm::mat2(cos(dir), sin(dir), -sin(dir), cos(dir));
    offset = rotation * offset;
    newPosition += offset;

    glm::vec2 aux = bloon_pos - newPosition;
    float newDir = atan2(aux.y, aux.x);

    projectiles->push_back(new Projectile(newPosition, newDir, 500, this->penetration));
}

void DartMonkey::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("dart_monkey");
    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), glm::vec2(spriteTex.Width, spriteTex.Height), this->dir*(180/M_PI), glm::vec3(1.0f, 1.0f, 1.0f), true);
}

//Tack Tower
TackShooter::TackShooter(glm::vec2 pos) : Monkey(pos)
{
    std::ifstream ifs("data/towers/tack_shooter/stats.json");
    Json::Value stats_json;
    ifs >> stats_json;
    this->cost = stats_json["cost"].asInt();
    this->sell_value = stats_json["sell_value"].asInt();
    this->cooldown = stats_json["cooldown"].asFloat();
    this->range = stats_json["range"].asInt();
    this->penetration = stats_json["penetration"].asInt();
    this->size = stats_json["size"].asInt();
}

void TackShooter::shoot(glm::vec2 bloon_pos, std::vector<Projectile*>* projectiles)
{
    for(float i = 0; i < 8; i += 1.0)
    {
        projectiles->push_back(new Projectile(this->pos, (i * 2.0 * M_PI) / 8.0, 500, this->penetration));
    }
}

void TackShooter::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("tack_shooter");
    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), glm::vec2(spriteTex.Width, spriteTex.Height), 0, glm::vec3(1.0f, 1.0f, 1.0f), true);
}
