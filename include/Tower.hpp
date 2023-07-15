#pragma once

#include "Projectile.hpp"

enum TowerState
{
    CAN_SHOOT,
    COOLDOWN
};

enum TowerType
{
    DART_MONKEY,
    TACK_SHOTER,
    CANNON,
    ICE_MONKEY,
    SUPER_MONKEY
};

class Tower
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
    TowerState state;
    bool isExisting;
public:
	int up_1_cost, up_2_cost;
    bool selected;
    int size;
    TowerType type;
    Tower(glm::vec2);
	Tower(glm::vec2, std::string);
    Tower(glm::vec2, float, int);
    long get_id();
    glm::vec2 get_pos();
    int get_cost();
    int get_sell_price();
    int get_upgrade_cost(int);
    bool exists();
    virtual void upgrade(int) = 0;
    static void init();
    virtual void shoot(glm::vec2, std::vector<Projectile*>*) = 0;
    void update(float, std::vector<Bloon*>, std::vector<Projectile*>*);
    int sell();
    virtual void draw(SpriteRenderer*) = 0;
};

class DartTower : public Tower
{
protected:
    DartTower(glm::vec2, float, int);
public:
    DartTower(glm::vec2);
	DartTower(glm::vec2, std::string);
    void upgrade(int);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class Cannon : public Tower
{
public:
    Cannon(glm::vec2);
    void upgrade(int);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class TackShooter : public Tower
{
public:
    TackShooter(glm::vec2);
    void upgrade(int);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class IceTower : public Tower
{
public:
    IceTower(glm::vec2);
    void upgrade(int);
    void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};

class SuperTower : public DartTower
{
public:
    SuperTower(glm::vec2);
    //void upgrade(int);
    //void shoot(glm::vec2, std::vector<Projectile*>*);
    void draw(SpriteRenderer*);
};
