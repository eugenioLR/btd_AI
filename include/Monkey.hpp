#pragma once

#include "Projectile.hpp"

#include <json/json.h>

enum MonkeyState
{
    CAN_SHOOT,
    COOLDOWN
};

enum MonkeyType
{
    DART_MONKEY,
    TACK_SHOTER,
    CANNON,
    SUPER_MONKEY
};

class Monkey
{
protected:
    long id;
    glm::vec2 pos;
    int cost;
    //int up_1_cost, up_2_cost;
    int upgrade1, upgrade2;
    int sell_value;
    float dir;
    float cooldown;
    float timer;
    int range;
    int penetration;
    MonkeyState state;
    bool isExisting;
public:
	int up_1_cost, up_2_cost;
    bool selected;
    int size;
    MonkeyType type;
    Monkey(glm::vec2);
	Monkey(glm::vec2, std::string);
    Monkey(glm::vec2, float, int);
    glm::vec2 get_pos();
    int get_cost();
    int get_upgrade_cost(int);
    bool exists();
    virtual void upgrade(int) = 0;
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
	DartMonkey(glm::vec2, std::string);
    void upgrade(int);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class Cannon : public Monkey
{
public:
    Cannon(glm::vec2);
    void upgrade(int);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class TackShooter : public Monkey
{
public:
    TackShooter(glm::vec2);
    void upgrade(int);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class SuperMonkey : public DartMonkey
{
public:
    SuperMonkey(glm::vec2);
    //void upgrade(int);
    //void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};
