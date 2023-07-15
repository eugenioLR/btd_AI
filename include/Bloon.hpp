#pragma once

#include "opengl_classes/ResourceManager.hpp"

#include "../imgui/imgui.h"
#include "imgui_backends/imgui_impl_opengl3.h"
#include "imgui_backends/imgui_impl_glfw.h"

#include <png++/png.hpp>
#include <png++/image.hpp>
#include <png++/rgb_pixel.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <queue>
#include <string>
#include <set>
#include <bitset>
#include <json/json.h>
#include <algorithm>

#define WIDTH 700
#define HEIGHT 446
#define OFFSET_X 0
#define OFFSET_Y 0

enum BloonType 
{
    RED,
    BLUE,
    GREEN,
    YELLOW,
    BLACK,
    WHITE

};

enum DamageType 
{
    NO_DMG,
    PIERCE,
    FREEZE,
    EXPLOSION
};

class Bloon
{
private:
    long id;
	BloonType type;
    glm::vec2 pos;
    std::queue<glm::vec2> path;
    double size;
    int layers;
    float speed;
    double progress;
    bool is_frozen;
    bool finished;
    bool isExisting;
    std::vector<DamageType> resists;
public:
    Bloon();
    Bloon(glm::vec2, std::queue<glm::vec2>, BloonType, double);
	Bloon(std::queue<glm::vec2>, BloonType, double);
    // Bloon(std::queue<glm::vec2>, double, int, double);
    ~Bloon();
    static void init();
    long get_id();
	int get_type();
    glm::vec2 get_pos();
    double get_size();
    double get_progress();
    int get_layers();
    bool exists();
    void pop(DamageType);
    void draw(SpriteRenderer*);
    void update(float, std::vector<Bloon*>*);
};
