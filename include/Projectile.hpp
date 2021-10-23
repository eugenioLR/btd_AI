#pragma once

#include "Bloon.hpp"
#include <vector>


class Projectile
{
private:
    long id;
    glm::vec2 pos;
    std::set<long> hitBloons;
    float dir;
    float speed;
    int penetration;
    bool isExisting;
public:
    Projectile(glm::vec2, float, float, int);
    ~Projectile();
    bool exists();
    static void init();
    void update(float, std::vector<Bloon*>, int*);
    void draw(SpriteRenderer*);
};
