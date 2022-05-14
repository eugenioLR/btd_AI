#pragma once

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
    MonkeyType m_type;
    Monkey* selected;
    SpriteRenderer* sRenderer;
    std::vector<Bloon*> bloons;
    std::vector<Monkey*> monkeys;
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
    void addMonkey(glm::vec2, MonkeyType);
    void addMonkey(glm::vec2, MonkeyType, bool);
    void addBloon();
    void init();
    void main_loop();
    void no_graphics_loop();
    void handle_events();
    void handle_gui_events();
    void logic(double deltatime);
    void drawGUI();
    void draw(double deltatime);
    void cleanup();
};
