#pragma once

#include "ResourceManager.hpp"

#define WIDTH 650
#define HEIGHT 446
#define OFFSET_X 0
#define OFFSET_Y 0

class Bloon
{
private:
    long id;
    glm::vec2 pos;
    std::queue<glm::vec2> path;
    double size;
    int layers;
    float speed;
    double progress;
    bool is_frozen;
    bool finished;
    bool isExisting;
public:
    Bloon();
    Bloon(std::queue<glm::vec2>, double, int, double);
    ~Bloon();
    static void init();//SDL_Renderer*);
    long get_id();
    glm::vec2 get_pos();
    double get_size();
    double get_progress();
    int get_layers();
    bool exists();
    void pop();
    void draw(SpriteRenderer*);
    void update(float);
};
