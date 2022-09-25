#include "Tower.hpp"

long nexttowerid = 0;

//General Tower
Tower::Tower(glm::vec2 pos)
{
    this->id = nexttowerid++;
    this->pos = pos;
    this->timer = 0;
    this->upgrade1 = 0;
    this->upgrade2 = 0;
    this->dir = 3*M_PI/2;
    this->state = COOLDOWN;
    this->isExisting = true;
    this->selected = false;
}

Tower::Tower(glm::vec2 pos, std::string stats_path)
{
	std::ifstream ifs(stats_path);
    Json::Value stats_json;
    ifs >> stats_json;

    this->id = nexttowerid++;
    this->pos = pos;
	this->cost = stats_json["cost"].asInt();
	this->upgrade1 = 0;
    this->upgrade2 = 0;
	this->sell_value = stats_json["sell_value"].asInt();
	this->dir = 3*M_PI/2;
	this->cooldown = stats_json["cooldown"].asFloat();
	this->timer = 0;
	this->range = stats_json["range"].asInt();
    this->penetration = stats_json["penetration"].asInt();
    this->state = COOLDOWN;
    this->isExisting = true;
	this->up_1_cost = stats_json["up_1_cost"].asInt();
    this->up_2_cost = stats_json["up_2_cost"].asInt();
    this->selected = false;
    this->size = stats_json["size"].asInt();
}

Tower::Tower(glm::vec2 pos, float cooldown, int range)
{
    this->id = nexttowerid++;
    this->pos = pos;
    this->cooldown = cooldown;
    this->timer = 0;
    this->range = range;
    this->upgrade1 = 0;
    this->upgrade2 = 0;
    this->dir = 3*M_PI/2;
    this->state = COOLDOWN;
    this->isExisting = true;
    this->selected = false;
}

long Tower::get_id()
{
    return this->id;
}

glm::vec2 Tower::get_pos()
{
    return this->pos;
}


int Tower::get_cost()
{
    return this->cost;
}


int Tower::get_sell_price()
{
    return this->sell_value;
}


int Tower::get_upgrade_cost(int index)
{
    int price;
    switch (index) {
        case 1:
            price = up_1_cost;
            break;
        case 2:
            price = up_2_cost;
            break;
        default:
            price = 0;
    }
    return price;
}

bool Tower::exists()
{
    return this->isExisting;
}

void Tower::init()
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
            skin_name = "skin_monkey.png";
            break;
        }
        case 1:
        {
            skin_name = "skin_cow.png";
            break;
        }
        default:
        {
            skin_name = "skin_monkey.png";
        }
    }
    std::string a = "";
    ResourceManager::loadTexture(("data/towers/dart_monkey/" + skin_name).c_str(), true, "dart_monkey");
    ResourceManager::loadTexture(("data/towers/tack_shooter/" + skin_name).c_str(), true, "tack_shooter");
	ResourceManager::loadTexture(("data/towers/cannon/" + skin_name).c_str(), true, "cannon");
    ResourceManager::loadTexture(("data/towers/super_monkey/" + skin_name).c_str(), true, "super_monkey");
}

void Tower::update(float deltatime, std::vector<Bloon*> bloons, std::vector<Projectile*>* projectiles)
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

int Tower::sell()
{
    this->isExisting = false;
    return this->sell_value;
}

//Dart Tower
DartTower::DartTower(glm::vec2 pos, float cooldown, int range) : Tower(pos, cooldown, range)
{
    this->cost = 250;
    this->sell_value = 200;
}

DartTower::DartTower(glm::vec2 pos, std::string stats_path) : Tower(pos, stats_path)
{
    this->type = DART_MONKEY;
}

DartTower::DartTower(glm::vec2 pos) : Tower(pos, "data/towers/dart_monkey/stats.json")
{
    this->type = DART_MONKEY;
}

void DartTower::upgrade(int index)
{
    switch (index) {
        case 1:
        {
            upgrade1++;
            this->penetration++;
            break;
        }
        case 2:
        {
            upgrade2++;
            this->range += this->range/4;
            break;
        }
    }
}

void DartTower::shoot(glm::vec2 bloon_pos, std::vector<Projectile*>* projectiles)
{
    glm::vec2 newPosition = this->pos;
    glm::vec2 offset = glm::vec2(20, 20);
    glm::mat2 rotation = glm::mat2(cos(dir), sin(dir), -sin(dir), cos(dir));
    offset = rotation * offset;
    newPosition += offset;

    glm::vec2 aux = bloon_pos - newPosition;
    float newDir = atan2(aux.y, aux.x);

    projectiles->push_back(new Dart(newPosition, newDir, 500, this->penetration));
}

void DartTower::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("dart_monkey"),
              circleTex = ResourceManager::getTexture("circle");
    glm::vec3 color;
    if(this->selected)
    {
        renderer->drawSprite(circleTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), ((float) range)/256, 0, glm::vec3(1.0f, 1.0f, 1.0f), true);
        color = glm::vec3(0.9f, 0.9f, 0.9f);
    }
    else
    {
        color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    
    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, this->dir*(180/M_PI), color, true);
}

//Super Tower

SuperTower::SuperTower(glm::vec2 pos) : DartTower(pos, std::string("data/towers/super_monkey/stats.json"))
{
    this->type = SUPER_MONKEY;
}

/*void DartTower::upgrade(int index)
{
    switch (index) {
        case 1:
        {
            upgrade1++;
            this->penetration++;
            break;
        }
        case 2:
        {
            upgrade2++;
            this->range += this->range/4;
            break;
        }
    }
}*/

/*
void DartTower::shoot(glm::vec2 bloon_pos, std::vector<Projectile*>* projectiles)
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
*/

void SuperTower::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("super_monkey"),
              circleTex = ResourceManager::getTexture("circle");
    glm::vec3 color;
    if(this->selected)
    {
        renderer->drawSprite(circleTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), ((float) range)/256, 0, glm::vec3(1.0f, 1.0f, 1.0f), true);
        color = glm::vec3(0.9f, 0.9f, 0.9f);
    }
    else
    {
        color = glm::vec3(1.0f, 1.0f, 1.0f);
    }


    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, this->dir*(180/M_PI), glm::vec3(1.0f, 1.0f, 1.0f), true);
}

//Tack Tower
TackShooter::TackShooter(glm::vec2 pos) : Tower(pos)
{
    std::ifstream ifs("data/towers/tack_shooter/stats.json");
    Json::Value stats_json;
    ifs >> stats_json;
    this->cost = stats_json["cost"].asInt();
    this->up_1_cost = stats_json["up_1_cost"].asInt();
    this->up_2_cost = stats_json["up_2_cost"].asInt();
    this->sell_value = stats_json["sell_value"].asInt();
    this->cooldown = stats_json["cooldown"].asFloat();
    this->range = stats_json["range"].asInt();
    this->penetration = stats_json["penetration"].asInt();
    this->size = stats_json["size"].asInt();
    this->type = TACK_SHOTER;
}

void TackShooter::upgrade(int index)
{
    switch (index) {
        case 1:
        {
            upgrade1++;
            this->penetration++;
            break;
        }
        case 2:
        {
            upgrade2++;
            this->range += this->range/4;
            break;
        }
    }
}

void TackShooter::shoot(glm::vec2 bloon_pos, std::vector<Projectile*>* projectiles)
{
    for(float i = 0; i < 8; i += 1.0)
    {
        projectiles->push_back(new Dart(this->pos, (i * 2.0 * M_PI) / 8.0, 500, this->penetration));
    }
}

void TackShooter::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("tack_shooter"),
              circleTex = ResourceManager::getTexture("circle");
    glm::vec3 color;
    if(this->selected)
    {
        renderer->drawSprite(circleTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), ((float) range)/256, 0, glm::vec3(1.0f, 1.0f, 1.0f), true);
        color = glm::vec3(0.9f, 0.9f, 0.9f);
    }
    else
    {
        color = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, 0, color, true);
}

//Tack Tower
Cannon::Cannon(glm::vec2 pos) : Tower(pos)
{
    std::ifstream ifs("data/towers/cannon/stats.json");
    Json::Value stats_json;
    ifs >> stats_json;
    this->cost = stats_json["cost"].asInt();
    this->up_1_cost = stats_json["up_1_cost"].asInt();
    this->up_2_cost = stats_json["up_2_cost"].asInt();
    this->sell_value = stats_json["sell_value"].asInt();
    this->cooldown = stats_json["cooldown"].asFloat();
    this->range = stats_json["range"].asInt();
    this->penetration = stats_json["penetration"].asInt();
    this->size = stats_json["size"].asInt();
    this->type = CANNON;
}

void Cannon::upgrade(int index)
{
    switch (index) {
        case 1:
        {
            upgrade1++;
            this->penetration++;
            break;
        }
        case 2:
        {
            upgrade2++;
            this->range += this->range/4;
            break;
        }
    }
}

void Cannon::shoot(glm::vec2 bloon_pos, std::vector<Projectile*>* projectiles)
{
    projectiles->push_back(new Bomb(this->pos, dir, 500, this->penetration));
}

void Cannon::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("cannon"),
              circleTex = ResourceManager::getTexture("circle");
    glm::vec3 color;
    if(this->selected)
    {
        renderer->drawSprite(circleTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), ((float) range)/256, 0, glm::vec3(1.0f, 1.0f, 1.0f), true);
        color = glm::vec3(0.9f, 0.9f, 0.9f);
    }
    else
    {
        color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    
    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, this->dir*(180/M_PI), color, true);
}
