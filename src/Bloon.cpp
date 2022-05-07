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

Bloon::Bloon(std::queue<glm::vec2> path, int type, double speed)
{
	this->id = nextbloonid++;
    this->pos = path.front();
    this->path = path;
	this->type = type;
	switch(type)
	{
		case 0:
		{
			this->size = 25;
		    this->layers = 1;
			break;
		}

		case 1:
		{
			this->size = 25;
		    this->layers = 2;
			break;
		}

		case 2:
		{
			this->size = 30;
		    this->layers = 3;
			break;
		}

		default:
		{
			this->size = 25;
		    this->layers = 0;
		}
	}

	this->speed = speed;
    this->progress = 0;
    this->is_frozen = false;
    this->isExisting = true;
}

Bloon::Bloon(std::queue<glm::vec2> path, double size, int layers, double speed)
{
    this->id = nextbloonid++;
    this->pos = path.front();
    this->path = path;
	this->type = 0;
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
	ResourceManager::loadTexture("data/enemies/pop.png", true, "bloon_pop");
	ResourceManager::loadTexture("data/enemies/red.png", true, "bloon_red");
	ResourceManager::loadTexture("data/enemies/blue.png", true, "bloon_blue");
	ResourceManager::loadTexture("data/enemies/green.png", true, "bloon_green");
	ResourceManager::loadTexture("data/enemies/unknown.png", true, "bloon_unknown");
}

long Bloon::get_id()
{
    return this->id;
}

int Bloon::get_type()
{
	return this->type;
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

int Bloon::get_layers()
{
    return this->layers;
}

void Bloon::pop()
{
    this->layers--;
}

void Bloon::update(float deltatime)
{
	float realspeed = (1.05 - 0.05 * layers) * speed;
    glm::vec2 direction = path.front() - this->pos;
    glm::vec2 step = deltatime * realspeed * glm::normalize(direction);
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
    Texture2D spriteTex;
	switch(this->layers)
	{
		case 0:
			spriteTex = ResourceManager::getTexture("bloon_pop");
			break;

		case 1:
			spriteTex = ResourceManager::getTexture("bloon_red");
			break;

		case 2:
			spriteTex = ResourceManager::getTexture("bloon_blue");
			break;

		case 3:
			spriteTex = ResourceManager::getTexture("bloon_green");
			break;

		default:
			spriteTex = ResourceManager::getTexture("bloon_unknown");
	}
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, 0, color, true);
}
