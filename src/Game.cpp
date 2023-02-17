#include "Game.hpp"

//---------DELETE THIS----------
//int bloon_rate = 60;
//int Ttime = 0;
int bloon_layers = 1;
float bloon_speed = 300;
//bool debug_active = false;
//int new_round = 0;

//
Game::Game(int width, int height)
{
    this->width = width;
    this->height = height;
    this->money = 650;
    //this->health = 40;
    this->health = 4000;
    this->round = 1;
    this->state = IDLE;
    this->m_type = DART_MONKEY;
}

Game::~Game()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glDeleteShader(shaderProgram);
    delete map_layout;
}

void Game::increase_money(int amount)
{
    money += amount;
}

void Game::decrease_money(int amount)
{
    if((money -= amount) < 0)
        money = 0;
}

void Game::addTower(glm::vec2 pos, TowerType m_type)
{
    addTower(pos, m_type, false);
}

void Game::addTower(glm::vec2 pos, TowerType m_type, bool is_free)
{
    Tower* tower;
    switch (m_type)
    {
        case DART_MONKEY:
            tower = new DartTower(pos);
            break;
        case TACK_SHOTER:
            tower = new TackShooter(pos);
            break;
        case CANNON:
			tower = new Cannon(pos);
			break;
		case SUPER_MONKEY:
			tower = new SuperTower(pos);
			break;
        default:
            tower = new DartTower(pos);
    }
    if(map_layout->canPlace(pos, tower->size)){
        if(this->money >= tower->get_cost() || is_free)
        {
            this->towers.push_back(tower);
            if(!is_free)
                this->money -= tower->get_cost();
            map_layout->place(pos, tower->size);
        }
    }
}

void Game::sellTower(int idx)
{
    this->sellTower(this->towers[idx]);
}

void Game::sellTower(Tower* tower)
{
    int money_back = tower->sell();

    this->increase_money(money_back);
    this->map_layout->reset_placing_map();
    for(int i = 0; i < this->towers.size(); i++)
    {
        if(towers[i]->get_id() != tower->get_id())
        {
            this->map_layout->place(towers[i]->get_pos(), towers[i]->size);
        }
    }
}

void Game::addBloon()
{
    Bloon* bloon = new Bloon(this->map_layout->get_path(), 25, bloon_layers, bloon_speed);
    this->bloons.push_back(bloon);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "BTD AI", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cerr << "Failed to initialize GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    this->io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    ImGui::StyleColorsDark();

    ResourceManager::loadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    this->sRenderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

    this->map_layout = new MapLayout("data/maps/map1/skin.png", "data/maps/map1/placing_map.png", "data/maps/map1/info.json", "data/maps/map1/rounds.json");
    ResourceManager::loadTexture("data/misc/circle.png", true, "circle");
    ResourceManager::loadTexture("data/misc/circle_solid.png", true, "circle_solid");
    Bloon::init();
    Tower::init();
    Projectile::init();
}

void Game::main_loop()
{
    //double deltatime = 1.0/60.0;
    double deltatime, lastFrame, currentFrame = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handle_events();
        logic(deltatime);
        draw(deltatime);
        cleanup();
    }
    glfwTerminate();
}

void Game::perform_action()
{
    // Do nothing, place tower, upgrade tower or sell tower
}

void Game::step()
{
    double deltatime = 1.0/60;

    logic(deltatime);

    cleanup();
}

GameState Game::get_gamestate()
{
    GameState game_info;
    game_info.round = this->round;
    game_info.money = this->money;
    game_info.health = this->health;

    game_info.bloon_count = this->bloons.size();
    game_info.tower_amount = this->towers.size();
    game_info.bloon_progress = 0;

    return game_info;
}

void Game::handle_events()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    glfwPollEvents();
}

// void Game::learn(Actor* actor){
//     Action a = actor->choose_action(this->towers, this->bloons, this->money, this->health);
//     std::cout << a.action_name << std::endl;

//     actor->update_reward(this->bloons, this->money, this->health);
// }

void Game::logic(double deltatime)
{
    for(int i = 0; i < towers.size(); i++)
        towers[i]->update(deltatime, bloons, &projectiles);

    for(int i = 0; i < projectiles.size(); i++)
        projectiles[i]->update(deltatime, bloons, &projectiles, &money);

    for(int i = 0; i < bloons.size(); i++)
        bloons[i]->update(deltatime);

	map_layout->update(deltatime, &bloons);
}

void Game::drawGUI()
{
	std::string text;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //layout
    ImGui::Begin("    ");

    ImGui::SetWindowPos(ImVec2(485, 0));
    ImGui::SetWindowSize(ImVec2(215, 446));

	int curr_round = std::max(map_layout->get_round(), 0);
	text = "ROUND " + std::to_string(curr_round);
    ImGui::TextUnformatted(text.c_str());
    // text = "money: " + std::to_string(money) + "$";
    // ImGui::TextUnformatted(text.c_str());
    // text = "health: " + std::to_string(health);
    // ImGui::TextUnformatted(text.c_str());
    text = "money: " + std::to_string(money) + "$\thealth: " + std::to_string(health);
    ImGui::TextUnformatted(text.c_str());


    if(ImGui::Button("Dart tower:   250$"))
    {
        this->m_type = DART_MONKEY;
        if(state != HOLDING_MONKEY)
            state = HOLDING_MONKEY;
    }

    if(ImGui::Button("Tack Shooter:  400$"))
    {
        this->m_type = TACK_SHOTER;
        if(state != HOLDING_MONKEY)
            state = HOLDING_MONKEY;
    }

    if(ImGui::Button("Ice:           800$"))
    {
        this->m_type = TACK_SHOTER;
        if(state != HOLDING_MONKEY)
            state = HOLDING_MONKEY;
    }

    if(ImGui::Button("Cannon:        900$"))
    {
        this->m_type = CANNON;
        if(state != HOLDING_MONKEY)
            state = HOLDING_MONKEY;
    }

    if(ImGui::Button("Super tower: 4000$"))
    {
        this->m_type = SUPER_MONKEY;
        if(state != HOLDING_MONKEY)
            state = HOLDING_MONKEY;
    }

    //other
    switch(state)
    {
        case HOLDING_MONKEY:
        {
            Texture2D spriteTex, circleTex, circleSolidTex;
            int cost, size, range;
            if(selected != nullptr)
                selected->selected = false;
            
            circleTex = ResourceManager::getTexture("circle");
            circleSolidTex = ResourceManager::getTexture("circle_solid");
            switch(m_type)
            {
                case DART_MONKEY:
                {
                    spriteTex = ResourceManager::getTexture("dart_monkey");
                    std::ifstream ifs("data/towers/dart_monkey/stats.json");
                    Json::Value stats_json;
                    ifs >> stats_json;
                    range = stats_json["range"].asInt();
                    cost = stats_json["cost"].asInt();
                    size = stats_json["size"].asInt();
                    break;
                }
                case TACK_SHOTER:
                {
                    spriteTex = ResourceManager::getTexture("tack_shooter");
                    std::ifstream ifs("data/towers/tack_shooter/stats.json");
                    Json::Value stats_json;
                    ifs >> stats_json;
                    range = stats_json["range"].asInt();
                    cost = stats_json["cost"].asInt();
                    size = stats_json["size"].asInt();
                    break;
                }
                case CANNON:
                {
                    spriteTex = ResourceManager::getTexture("cannon");
                    std::ifstream ifs("data/towers/cannon/stats.json");
                    Json::Value stats_json;
                    ifs >> stats_json;
                    range = stats_json["range"].asInt();
                    cost = stats_json["cost"].asInt();
                    size = stats_json["size"].asInt();
                    break;
                }
                case SUPER_MONKEY:
                {
                    spriteTex = ResourceManager::getTexture("super_monkey");
                    std::ifstream ifs("data/towers/super_monkey/stats.json");
                    Json::Value stats_json;
                    ifs >> stats_json;
                    range = stats_json["range"].asInt();
                    cost = stats_json["cost"].asInt();
                    size = stats_json["size"].asInt();
                    break;
                }
                default:
                {
                    spriteTex = ResourceManager::getTexture("super_monkey");
                    cost = -1;
                    size = -1;
                }
            }
            ImVec2 mousePos = ImGui::GetMousePos();
            glm::vec3 color;

            if(cost <= this->money && map_layout->canPlace(glm::vec2(mousePos.x, mousePos.y), size))
                color = glm::vec3(1.0f, 1.0f, 1.0f);
            else
            {
                color = glm::vec3(1.0f, 0.2f, 0.2f);
                if(cost > this->money)
                    ImGui::TextUnformatted("Not enough money");
                
                if(!map_layout->canPlace(glm::vec2(mousePos.x, mousePos.y), size))
                    ImGui::TextUnformatted("Can't be placed there");
            }

            // Draw range
            sRenderer->drawSprite(circleTex, glm::vec2(mousePos.x, mousePos.y), ((float) range)/256, 0, color, true);

            // Draw tower sprite
            sRenderer->drawSprite(spriteTex, glm::vec2(mousePos.x, mousePos.y), 1, 0, color, true);

            // Draw space occupied
            if(debug_active)
                sRenderer->drawSprite(circleSolidTex, glm::vec2(mousePos.x, mousePos.y), ((float) size)/256, 0, color, true);

            if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                this->state = IDLE;
                addTower(glm::vec2(mousePos.x, mousePos.y), m_type);
            }
            break;
        }

        case IDLE:
        {
            Tower* aux = nullptr;
            ImVec2 mousePos = ImGui::GetMousePos();
            glm::vec2 mouse = glm::vec2(mousePos.x, mousePos.y);
            float dist;
            if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                float min_dist = 10e100;
                for(int i = 0; i < towers.size(); i++)
                {
                    dist = glm::length(towers[i]->get_pos() - mouse);
                    if(dist < towers[i]->size && dist < min_dist)
                    {
                        min_dist = dist;
                        aux = towers[i];
                    }
                }

                if (aux != nullptr)
                {
                    this->state = MONKEY_INFO;
                    if(selected != nullptr)
                        selected->selected = false;
                    aux->selected = true;
                }

                selected = aux;
            }
            break;
        }

        case MONKEY_INFO:
        {
            Tower* aux = nullptr;
            ImVec2 mousePos = ImGui::GetMousePos();
            glm::vec2 mouse = glm::vec2(mousePos.x, mousePos.y);
            float dist;
            bool click_outside = false;

            ImGui::TextUnformatted("\n");

            std::string tower_type;

            switch(selected->type)
            {
                case DART_MONKEY:
                    tower_type = "Dart tower";
                    break;
                case TACK_SHOTER:
                    tower_type = "Tack shooter";
                    break;
                case CANNON:
                    tower_type = "Cannon";
                    break;
                case SUPER_MONKEY:
                    tower_type = "Super tower";
                    break;
                default:
                    tower_type = "???";

            }

            // Show tower name
            ImGui::TextUnformatted(tower_type.c_str());
            ImGui::TextUnformatted("Updates:\n");

            // Show first update info
			text = "Price: " + std::to_string(selected->up_1_cost) + "$";
			ImGui::TextUnformatted("\nPenetration");
            ImGui::TextUnformatted(text.c_str());
            if(ImGui::Button("Upgrade 1"))
            {
                if(selected->get_upgrade_cost(1) < money)
                {
                    money -= selected->get_upgrade_cost(1);
                    selected->upgrade(1);
                }
            }

            // Show first update info
			text = "Price: " + std::to_string(selected->up_2_cost) + "$";
			ImGui::TextUnformatted("\nRange");
            ImGui::TextUnformatted(text.c_str());
            if(ImGui::Button("Upgrade 2"))
            {
                if(selected->get_upgrade_cost(2) < money)
                {
                    money -= selected->get_upgrade_cost(2);
                    selected->upgrade(2);
                }
            }

            // Show sell info
            text = "\nSell price: " + std::to_string(selected->get_sell_price()) + "$";
            ImGui::TextUnformatted(text.c_str());
            if(ImGui::Button("Sell"))
            {
                this->sellTower(selected);
                this->state = IDLE;
            }


            Texture2D spriteTex = ResourceManager::getTexture("map_skin");
            if(mouse.x < spriteTex.Width)
            {
                if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    float min_dist = 10e100;
                    for(int i = 0; i < towers.size() && selected != nullptr; i++)
                    {
                        dist = glm::length(towers[i]->get_pos() - mouse);
                        if(dist < towers[i]->size + 4 && dist < min_dist)
                        {
                            min_dist = dist;
                            aux = towers[i];
                        }
                    }

                    selected->selected = false;
                    if (aux == nullptr)
                    {
                        this->state = IDLE;
                    }
                    else
                    {
                        aux->selected = true;
                    }

                    selected = aux;

                }
            }
            break;
        }
    }


    bool round_disable = this->map_layout->is_round_running();

    // Start round Button
    if(round_disable)
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(64,64,64,255));

	if(ImGui::Button("START ROUND") && !round_disable)
		this->map_layout->start_round();

    if(round_disable)
        ImGui::PopStyleColor();

    // Debug mode toggle
    ImGui::TextUnformatted("\n\n");
    if(ImGui::Button("DEBUG"))
        debug_active = !debug_active;

    if(debug_active)
    {
        // Round Override
		ImGui::SliderInt("Round", &new_round, 1, 10000);
		if(ImGui::Button("Override Round"))
		{
			this->map_layout->override_round(new_round);
		}

        // Round Stop
        if(ImGui::Button("Stop Round"))
		{
			this->map_layout->stop_round();
		}

        // Clear Bloons
		if(ImGui::Button("clear bloons"))
        {
            while(!this->bloons.empty())
                bloons.erase(bloons.begin());
        }

        // Clear Towers
        if(ImGui::Button("clear towers"))
        {
            while(!this->towers.empty())
                towers.erase(towers.begin());
            map_layout->reset_placing_map();
        }

        // Add money
        if(ImGui::Button("add 100$"))
        {
            money += 100;
        }

        if(ImGui::Button("add 1000$"))
        {
            money += 1000;
        }

        // Debug info
		text = "Bloons on sceen: " + std::to_string(bloons.size());
		ImGui::TextUnformatted(text.c_str());

		text = "Towers on sceen: " + std::to_string(towers.size());
		ImGui::TextUnformatted(text.c_str());

		text = "Projectiles on sceen: " + std::to_string(projectiles.size());
		ImGui::TextUnformatted(text.c_str());
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::draw(double deltatime)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //game objects
    map_layout->draw(sRenderer);

    for(int i = 0; i < bloons.size(); i++)
        bloons[i]->draw(sRenderer);

    for(int i = 0; i < projectiles.size(); i++)
        projectiles[i]->draw(sRenderer);

    for(int i = 0; i < towers.size(); i++)
        towers[i]->draw(sRenderer);

    drawGUI();

    glfwSwapBuffers(window);
}

void Game::cleanup()
{
    for(int i = 0; i < towers.size(); i++)
    {
        if(!towers[i]->exists())
        {
            towers.erase(towers.begin() + i);
            i--;
        }
    }

    for(int i = 0; i < projectiles.size(); i++)
    {
        if(!projectiles[i]->exists())
        {
            projectiles.erase(projectiles.begin() + i);
            i--;
        }
    }

    for(int i = 0; i < bloons.size(); i++)
    {
        if(!bloons[i]->exists())
        {
			this->map_layout->bloon_popped(*bloons[i], &money);
            if(health > 0 && bloons[i]->get_layers() > 0)
                health -= bloons[i]->get_layers();
            
            health = health > 0 ? health : 0;

            bloons.erase(bloons.begin() + i);
            i--;
        }
    }
}
