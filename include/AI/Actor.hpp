#pragma once

#include <glm/glm.hpp>
#include "imgui.h"
#include "Tower.hpp"

enum ActionName{
    Place,
    Sell,
    Upgrade,
    Wait
};

typedef struct {
    ActionName action_name;
    glm::vec2 pos;
    int tower_idx;
    std::map<ActionName, float> table;
} Action;

class Actor{
private:

public:
    Actor();
    Action choose_action(std::vector<Tower*> towers, std::vector<Bloon*> enemies, int money, int health);
    void update_reward(std::vector<Bloon*> enemies, int money, int health);

};