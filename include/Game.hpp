#pragma once

//#include "AI/Actor.hpp"
#include "MapLayout.hpp"

enum GUI_state
{
    IDLE,
    MONKEY_INFO,
    HOLDING_MONKEY
};

struct GameState
{
    int round;
    int money;
    int health;

    int bloon_count;
    int tower_amount;
    float bloon_progress;
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
    bool use_graphics;
public:
    MapLayout* map_layout;
    Game();
    Game(int, int, bool);
    ~Game();
    void reset();
    void increase_money(int);
    void decrease_money(int);
    void addTower(glm::vec2, TowerType);
    void addTower(glm::vec2, TowerType, bool);
    void sellTower(int);
    void sellTower(Tower*);
    void addBloon();
    void switch_debug();
    void init();
    void main_loop();

    void perform_action();
    void step();
    GameState get_gamestate();

    //void learn(Actor*);
    void handle_events();
    void handle_gui_events();
    void logic(double deltatime);
    void drawGUI();
    void draw(double deltatime);
    void cleanup();

};
