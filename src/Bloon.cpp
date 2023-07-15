#include "Bloon.hpp"

long nextbloonid = 0;

Bloon::Bloon()
{
    this->id = nextbloonid++;
    this->pos = glm::vec2(0.0,0.0);
    this->layers = 1;
    this->speed = 1;
    this->progress = 0;
    this->frozen_counter = 0;
    this->is_frozen = false;
    this->isExisting = true;
}

Bloon::Bloon(glm::vec2 pos, std::queue<glm::vec2> path_left, BloonType type, double speed)
{
	this->id = nextbloonid++;
    this->pos = pos;
    this->path = path_left;
	this->type = type;
	this->resists = {DamageType::NO_DMG};
	switch(type)
	{
		case BloonType::RED:
		{
			this->size = 25;
		    this->layers = 1;
			break;
		}

		case BloonType::BLUE:
		{
			this->size = 25;
		    this->layers = 2;
			break;
		}

		case BloonType::GREEN:
		{
			this->size = 30;
			this->layers = 3;
			break;
		}

		case BloonType::YELLOW:
		{
			this->size = 35;
			this->layers = 4;
			break;
		}

		case BloonType::WHITE:
		{
			this->resists.push_back(DamageType::FREEZE);
			this->size = 25;
		    this->layers = 9;
			break;
		}

		case BloonType::BLACK:
		{
			this->resists.push_back(DamageType::EXPLOSION);
			this->size = 25;
		    this->layers = 9;
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


Bloon::Bloon(std::queue<glm::vec2> path, BloonType type, double speed) : Bloon(path.front(), path, type, speed) 
{
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
	ResourceManager::loadTexture("data/enemies/yellow.png", true, "bloon_yellow");
	ResourceManager::loadTexture("data/enemies/white.png", true, "bloon_white");
	ResourceManager::loadTexture("data/enemies/black.png", true, "bloon_black");
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

void Bloon::pop(DamageType dmg_type)
{
	if(std::find(this->resists.begin(), this->resists.end(), dmg_type) == this->resists.end())
	{
		if(dmg_type == DamageType::FREEZE)
		{
			this->is_frozen = true;
			this->frozen_counter = 1.5;
		}
		else
		{
			this->layers--;
		}
	}
}

void Bloon::update(float deltatime, std::vector<Bloon*>* bloons)
{
	float type_int;
	if(this->type==BloonType::WHITE)
	{
		type_int = 1.05*static_cast<int>(BloonType::BLACK);
	}
	else
	{
		type_int = static_cast<int>(this->type);
	}
	
	float speed_penalty = 0.05 * type_int;
	float realspeed = (1.05 - speed_penalty) * speed;
    glm::vec2 direction = path.front() - this->pos;
    glm::vec2 step = deltatime * realspeed * glm::normalize(direction);
	if(this->is_frozen)
	{

	}
    else if(glm::length(direction) > glm::length(step)*1.25 && !this->path.empty())
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

	this->frozen_counter -= deltatime;
	if(this->frozen_counter <= 0)
	{
		this->is_frozen = false;
	}


	Bloon* new_bloon;
	std::queue<glm::vec2> path_left = this->path;
    if(layers < 1)
	{
		this->isExisting = false;
		switch(this->type)
		{
			case BloonType::BLUE:
			{
				new_bloon = new Bloon(this->pos, this->path, BloonType::RED, this->speed);
				bloons->push_back(new_bloon);
				break;
			}

			case BloonType::GREEN:
			{
				new_bloon = new Bloon(this->pos, this->path, BloonType::BLUE, this->speed);
				bloons->push_back(new_bloon);
				break;
			}

			case BloonType::YELLOW:
			{
				new_bloon = new Bloon(this->pos, this->path, BloonType::GREEN, this->speed);
				bloons->push_back(new_bloon);
				break;
			}

			case BloonType::BLACK:
			case BloonType::WHITE:
			{
				new_bloon = new Bloon(this->pos, this->path, BloonType::YELLOW, this->speed);
				bloons->push_back(new_bloon);
				new_bloon = new Bloon(this->pos - 4.0f * step, this->path, BloonType::YELLOW, this->speed);
				bloons->push_back(new_bloon);
				break;
			}

			default:
				break;
		}
	}
}


void Bloon::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex;
	if(this->layers < 1)
	{
		spriteTex = ResourceManager::getTexture("bloon_pop");
	}
	else
	{
		switch(this->type)
		{
			case BloonType::RED:
				spriteTex = ResourceManager::getTexture("bloon_red");
				break;

			case BloonType::BLUE:
				spriteTex = ResourceManager::getTexture("bloon_blue");
				break;

			case BloonType::GREEN:
				spriteTex = ResourceManager::getTexture("bloon_green");
				break;
			
			case BloonType::YELLOW:
				spriteTex = ResourceManager::getTexture("bloon_yellow");
				break;
			
			case BloonType::BLACK:
				spriteTex = ResourceManager::getTexture("bloon_black");
				break;
			
			case BloonType::WHITE:
				spriteTex = ResourceManager::getTexture("bloon_white");
				break;

			default:
				spriteTex = ResourceManager::getTexture("bloon_unknown");
		}
	}
    glm::vec3 color;
	if(this->is_frozen)
	{
		// color = glm::vec3(0.85f, 0.94f, 1.0f);
		// color = glm::vec3(1.14f, 1.05f, 1.0f);
		color = glm::vec3(0.7f, 0.7f, 1.5f);
	}
	else
	{
		color = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	

    renderer->drawSprite(spriteTex, this->pos + glm::vec2(OFFSET_X, OFFSET_Y), 1, 0, color, true);
}
