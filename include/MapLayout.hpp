#pragma once

#include "Monkey.hpp"

class Game;

class MapLayout
{
private:
    std::queue<glm::vec2> bloon_path;
    std::bitset<WIDTH * HEIGHT> placing_map;
    std::string placing_path;
    void place_rec(int, int, int, int, std::set<int>*);
    void canPlace_rec(int, int, int, int, bool*, std::set<int>*);
public:
    MapLayout();
    ~MapLayout();
    MapLayout(std::string, std::string, std::string);
    std::queue<glm::vec2> get_path();
    void draw(SpriteRenderer*);
    void reset_placing_map();
    void place(glm::vec2, int);
    bool canPlace(glm::vec2, int);
};
