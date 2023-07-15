#pragma once

#include "Bloon.hpp"
#include <vector>

class Projectile
{
protected:
    long id;
    glm::vec2 pos;
    std::set<long> hitBloons;
    float dir;
    float speed;
    int penetration;
    float lifetime;
    bool isExisting;
    DamageType dmg_type;
public:
    Projectile(glm::vec2, DamageType, float, float, int, float);
    bool exists();
    static void init();
    virtual void update(float, std::vector<Bloon*>*, std::vector<Projectile*>*, int*) = 0;
    virtual void draw(SpriteRenderer*) = 0;
};

class Bomb : public Projectile
{
public:
    Bomb(glm::vec2, float, float, int);
    void update(float, std::vector<Bloon*>*, std::vector<Projectile*>*, int*);
    void draw(SpriteRenderer*);
};

class Explosion : public Projectile
{
public:
    Explosion(glm::vec2, float, float, int);
    void update(float, std::vector<Bloon*>*, std::vector<Projectile*>*, int*);
    void draw(SpriteRenderer*);
};

class FreezeArea : public Projectile
{
private:
    float area;
public:
    FreezeArea(glm::vec2, float, float, int);
    void update(float, std::vector<Bloon*>*, std::vector<Projectile*>*, int*);
    void draw(SpriteRenderer*);
};

class Dart : public Projectile
{
public:
    Dart(glm::vec2, float, float, int);
    void update(float, std::vector<Bloon*>*, std::vector<Projectile*>*, int*);
    void draw(SpriteRenderer*);
};
