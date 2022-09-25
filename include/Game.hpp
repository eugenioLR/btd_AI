#pragma once

#include "AI/Actor.hpp"
#include "MapLayout.hpp"

enum GUI_state
{
    IDLE,
    MONKEY_INFO,
    HOLDING_MONKEY
};

class Game
{
private:
    GLFWwindow* window;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;
    unsigned int VBO;
    unsigned int VBA;
    unsigned int EBO;
    ImGuiIO io;
    GUI_state state;
    TowerType m_type;
    Tower* selected;
    SpriteRenderer* sRenderer;
    std::vector<Bloon*> bloons;
    std::vector<Tower*> towers;
    std::vector<Projectile*> projectiles;
    int round;
    int money;
    int health;
    int width, height;
    bool debug_active = false;
    int new_round;
    bool quit;
public:
    MapLayout* map_layout;
    Game();
    Game(int, int);
    ~Game();
    void increase_money(int);
    void decrease_money(int);
    void addTower(glm::vec2, TowerType);
    void addTower(glm::vec2, TowerType, bool);
    void sellTower(int);
    void sellTower(Tower*);
    void addBloon();
    void init();
    void main_loop();
    void no_graphics_loop();
    void learn(Actor*);
    void handle_events();
    void handle_gui_events();
    void logic(double deltatime);
    void drawGUI();
    void draw(double deltatime);
    void cleanup();
};
