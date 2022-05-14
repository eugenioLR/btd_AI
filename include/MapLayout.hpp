#pragma once

#include "RoundGenerator.hpp"

class Game;

class MapLayout
{
private:
    std::queue<glm::vec2> bloon_path;
    std::bitset<WIDTH * HEIGHT> placing_map;
    std::string placing_path;
	RoundGenerator round_gen;
    void place_rec(int, int, int, int, int, std::set<int>*);
    void canPlace_rec(int, int, int, int, int, bool*, std::set<int>*);
public:
    MapLayout();
    ~MapLayout();
    MapLayout(std::string, std::string, std::string, std::string);
    std::queue<glm::vec2> get_path();
	void update(double, std::vector<Bloon*>*);
    void draw(SpriteRenderer*);
    void reset_placing_map();
    void place(glm::vec2, int);
    bool canPlace(glm::vec2, int);
	void bloon_popped(Bloon, int*);
	void start_round();
	void override_round(int);
	int get_round();
};
