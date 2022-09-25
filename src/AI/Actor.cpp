#include "Actor.hpp"

Actor::Actor(){
    //this->table = ;
}

Action Actor::choose_action(std::vector<Tower*> towers, std::vector<Bloon*> enemies, int money, int health){
    Action a;
    a.action_name = Place;
    a.pos = glm::vec2(1,1);
    a.tower_idx = 0;
    return a;
}

void Actor::update_reward(std::vector<Bloon*> enemies, int money, int health){
    // amount of enemies in screen
    float enemy_amount = (float) enemies.size();


    // progress of the enemy
    float enemy_progress = 0, max_progress = 0;
    Bloon* b;

    for(int i = 0; i < enemies.size(); i++)
    {
        b = enemies[i];
        if(max_progress < b->get_progress())
            max_progress = b->get_progress();
    }

    // "fitness" function
    // more money is better, more health is better, less enemies is better, less enemy progress is better
    //float fitness = 0.2*money + 2*health + 2/enemy_amount + 20/enemy_progress
    float fitness = 2*health + 2/enemy_amount + 20/enemy_progress;
    //this->table[this->last_action.action_name]

    
}