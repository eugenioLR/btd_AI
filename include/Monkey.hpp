#pragma once

#include "Projectile.hpp"

enum MonkeyState
{
    CAN_SHOOT,
    COOLDOWN
};

enum MonkeyType
{
    DART_MONKEY,
    TACK_SOOTER,
    CANNON,
    SUPER_MONKEY
};

class Monkey
{
protected:
    long id;
    glm::vec2 pos;
    int cost;
    int sell_value;
    float dir;
    float cooldown;
    float timer;
    int range;
    int penetration;
    MonkeyState state;
    int update1;
    int update2;
    bool isExisting;
public:
    int size;
    Monkey(glm::vec2);
    Monkey(glm::vec2, float, int);
    int get_cost();
    bool exists();
    static void init();
    virtual void shoot(glm::vec2, std::vector<Projectile*>*) = 0;
    void update(float, std::vector<Bloon*>, std::vector<Projectile*>*);
    virtual void draw(SpriteRenderer*) = 0;
};

class DartMonkey : public Monkey
{
protected:
    DartMonkey(glm::vec2, float, int);
public:
    DartMonkey(glm::vec2);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class Cannon : public Monkey
{
public:
    Cannon(glm::vec2);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class TackShooter : public Monkey
{
public:
    TackShooter(glm::vec2);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class SuperMonkey : public DartMonkey
{
public:
    SuperMonkey(glm::vec2);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};
