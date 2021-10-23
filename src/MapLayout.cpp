#include "MapLayout.hpp"

MapLayout::MapLayout()
{

}

MapLayout::MapLayout(std::string map_skin_path, std::string placing_path, std::string bloon_path_path, std::string map_rounds_path)
{
    //Load background image
    ResourceManager::loadTexture(map_skin_path.c_str(), true, "map_skin");

    this->placing_path = placing_path;

    png::image<png::rgb_pixel> image(placing_path);
    for (png::uint_32 y = 0; y < image.get_height(); ++y)
    {
        for (png::uint_32 x = 0; x < image.get_width(); ++x)
        {
            placing_map[y * image.get_width()  + x] = !!(image[y][x].red + image[y][x].blue + image[y][x].green);
        }
    }

    std::ifstream ifs(bloon_path_path);
    Json::Value path_json;
    ifs >> path_json;
    Json::Value path_points = path_json["path"];
    glm::vec2 point;
    for(int i = 0; i < path_points.size(); i++)
    {
        point = glm::vec2(path_points[i][0].asDouble(), path_points[i][1].asDouble());
        bloon_path.push(point);
    }

	this->round_gen = *(new RoundGenerator(map_rounds_path));
}

MapLayout::~MapLayout()
{
}

std::queue<glm::vec2> MapLayout::get_path()
{
	return this->bloon_path;
}

void MapLayout::update(double deltatime, std::vector<Bloon*>* bloon_list)
{
	this->round_gen.update(deltatime, bloon_list, this->bloon_path);
}

void MapLayout::draw(SpriteRenderer* renderer)
{
    Texture2D spriteTex = ResourceManager::getTexture("map_skin");
    renderer->drawSprite(spriteTex, glm::vec2(OFFSET_X, 0.0f), glm::vec2(spriteTex.Width, spriteTex.Height), 0, glm::vec3(1.0f, 1.0f, 1.0f), false);
    //sRenderer->drawSprite(spriteTex, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0, glm::vec3(0.0f, 1.0f, 0.0f));
}

void MapLayout::reset_placing_map()
{
    png::image<png::rgb_pixel> image(placing_path);
    for (png::uint_32 y = 0; y < image.get_height(); ++y)
    {
        for (png::uint_32 x = 0; x < image.get_width(); ++x)
        {
            placing_map[y * image.get_width()  + x] = !!(image[y][x].red + image[y][x].blue + image[y][x].green);
        }
    }
}

void MapLayout::place(glm::vec2 pos, int size)
{
    Texture2D spriteTex = ResourceManager::getTexture("map_skin");
    std::set<int> closed;
    place_rec(size, ((int) pos.y) * spriteTex.Width + ((int) pos.x), spriteTex.Width, spriteTex.Height, &closed);
}

void MapLayout::place_rec(int size, int pos, int width, int height, std::set<int>* closed)
{
    bool inBounds = pos >= 0 && pos < placing_map.size();
    if(size > 0 && inBounds)
    {
        if(closed->find(pos) == closed->end() && placing_map[pos])
        {
            placing_map[pos] = false;
            closed->insert(pos);
            std::vector<int> offsets;
            offsets.push_back(1);
            offsets.push_back(width);
            offsets.push_back(-width);
            offsets.push_back(-1);
            for(int offset : offsets)
            {
                place_rec(size - 1, pos + offset, width, height, closed);
            }
        }
    }
}

bool MapLayout::canPlace(glm::vec2 pos, int size)
{
    Texture2D spriteTex = ResourceManager::getTexture("map_skin");

    bool can_be_placed = pos.x >= 0 && pos.x < spriteTex.Width && pos.y >= 0 && pos.y < spriteTex.Height;

    if(can_be_placed)
    {
        std::set<int> closed;
        canPlace_rec(size, ((int) pos.y) * spriteTex.Width + ((int) pos.x), spriteTex.Width, spriteTex.Height, &can_be_placed, &closed);
    }

    return can_be_placed;
}

void MapLayout::canPlace_rec(int size, int pos, int width, int height, bool* can_be_placed, std::set<int>* closed)
{
    bool inBounds = pos >= 0 && pos < placing_map.size();
    if(size > 0 && (*can_be_placed) && inBounds)
    {
        *can_be_placed = placing_map[pos];
        if(closed->find(pos) == closed->end() && placing_map[pos])
        {
            closed->insert(pos);
            std::vector<int> offsets;
            offsets.push_back(1);
            offsets.push_back(width);
            offsets.push_back(-width);
            offsets.push_back(-1);
            for(int offset : offsets)
            {
                canPlace_rec(size - 1, pos + offset, width, height, can_be_placed, closed);
            }
        }
    }
}

void MapLayout::bloon_popped(Bloon bloon, int* money)
{
	this->round_gen.bloon_popped(bloon, money);
}

void MapLayout::start_round()
{
	this->round_gen.start_round();
}

void MapLayout::override_round(int new_round)
{
	this->round_gen.override_round(new_round);
}

int MapLayout::get_round()
{
	return this->round_gen.get_round();
}
