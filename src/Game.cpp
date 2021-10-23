#include "Game.hpp"

//---------DELETE THIS----------
int bloon_rate = 60;
int Ttime = 0;
int bloon_layers = 1;
float bloon_speed = 300;
bool debug_active = false;
int new_round = 0;

Game::Game(int width, int height)
{
    this->width = width;
    this->height = height;
    this->money = 650;
    //this->money = 500000;
    this->health = 40;
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

void Game::addMonkey(glm::vec2 pos, MonkeyType m_type)
{
    addMonkey(pos, m_type, false);
}

void Game::addMonkey(glm::vec2 pos, MonkeyType m_type, bool is_free)
{
    Monkey* monkey;
    switch (m_type)
    {
        case DART_MONKEY:
            monkey = new DartMonkey(pos);
            break;
        case TACK_SHOTER:
            monkey = new TackShooter(pos);
            break;
		case SUPER_MONKEY:
			monkey = new SuperMonkey(pos);
			break;
        default:
            monkey = new DartMonkey(pos);
    }
    if(map_layout->canPlace(pos, monkey->size)){
        if(this->money >= monkey->get_cost() || is_free)
        {
            this->monkeys.push_back(monkey);
            if(!is_free)
                this->money -= monkey->get_cost();
            map_layout->place(pos, monkey->size);
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

    this->map_layout = new MapLayout("data/map1/skin.png", "data/map1/placing_map.png", "data/map1/info.json", "data/map1/rounds.json");
    Bloon::init();
    Monkey::init();
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

void Game::no_graphics_loop()
{
    double deltatime= 1.0/60;

    quit = false;

    while(!quit)
    {
        handle_events();
        logic(deltatime);
        cleanup();
    }

    std::cout << "done" << std::endl;
}

void Game::handle_events()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    glfwPollEvents();
}

void Game::logic(double deltatime)
{
    for(int i = 0; i < monkeys.size(); i++)
        monkeys[i]->update(deltatime, bloons, &projectiles);

    for(int i = 0; i < projectiles.size(); i++)
        projectiles[i]->update(deltatime, bloons, &money);

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
    ImGui::Text(text.c_str());
    text = "money: " + std::to_string(money) + "$";
    ImGui::Text(text.c_str());
    text = "health: " + std::to_string(health);
    ImGui::Text(text.c_str());


    if(ImGui::Button("Dart monkey:   250$"))
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

    if(ImGui::Button("Super monkey: 4000$"))
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
            Texture2D spriteTex;
            int cost, size;
            if(selected != nullptr)
                selected->selected = false;
            switch(m_type)
            {
                case DART_MONKEY:
                {
                    spriteTex = ResourceManager::getTexture("dart_monkey");
                    std::ifstream ifs("data/towers/dart_monkey/stats.json");
                    Json::Value stats_json;
                    ifs >> stats_json;
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
                    cost = stats_json["cost"].asInt();
                    size = stats_json["size"].asInt();
                    break;
                }
                default:
                {
                    spriteTex = ResourceManager::getTexture("dart_monkey");
                    cost = 0;
                    size = 0;
                }
            }
            ImVec2 mousePos = ImGui::GetMousePos();
            glm::vec3 color;

            if(cost <= this->money && map_layout->canPlace(glm::vec2(mousePos.x, mousePos.y), size))
                color = glm::vec3(1.0f, 1.0f, 1.0f);
            else
                color = glm::vec3(1.0f, 0.3f, 0.3f);

            sRenderer->drawSprite(spriteTex, glm::vec2(mousePos.x, mousePos.y), glm::vec2(spriteTex.Width, spriteTex.Height), 0, color, true);
            if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                this->state = IDLE;
                addMonkey(glm::vec2(mousePos.x, mousePos.y), m_type);
            }
            break;
        }

        case IDLE:
        {
            Monkey* aux = nullptr;
            ImVec2 mousePos = ImGui::GetMousePos();
            glm::vec2 mouse = glm::vec2(mousePos.x, mousePos.y);
            float dist;
            if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                float min_dist = 10e100;
                for(int i = 0; i < monkeys.size(); i++)
                {
                    dist = glm::length(monkeys[i]->get_pos() - mouse);
                    if(dist < monkeys[i]->size && dist < min_dist)
                    {
                        min_dist = dist;
                        aux = monkeys[i];
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
            Monkey* aux = nullptr;
            ImVec2 mousePos = ImGui::GetMousePos();
            glm::vec2 mouse = glm::vec2(mousePos.x, mousePos.y);
            float dist;
            bool click_outside = false;

            ImGui::Text("\n");

            std::string monkey_type;

            switch(selected->type)
            {
                case DART_MONKEY:
                    monkey_type = "Dart monkey";
                    break;
                case TACK_SHOTER:
                    monkey_type = "Tack shooter";
                    break;
                case CANNON:
                    monkey_type = "Dart monkey";
                    break;
                case SUPER_MONKEY:
                    monkey_type = "Super monkey";
                    break;
                default:
                    monkey_type = "???";

            }


            ImGui::Text(monkey_type.c_str());


			text = "Price: " + std::to_string(selected->up_1_cost) + "$";
			ImGui::Text("Penetration");
            ImGui::Text(text.c_str());
            if(ImGui::Button("Upgrade 1"))
            {
                if(selected->get_upgrade_cost(1) < money)
                {
                    money -= selected->get_upgrade_cost(1);
                    selected->upgrade(1);
                }
            }

			text = "Price: " + std::to_string(selected->up_2_cost) + "$";
			ImGui::Text("Range");
            ImGui::Text(text.c_str());
            if(ImGui::Button("Upgrade 2"))
            {
                if(selected->get_upgrade_cost(2) < money)
                {
                    money -= selected->get_upgrade_cost(2);
                    selected->upgrade(2);
                }
            }

            Texture2D spriteTex = ResourceManager::getTexture("map_skin");
            if(mouse.x < spriteTex.Width)
            {
                if(ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    float min_dist = 10e100;
                    for(int i = 0; i < monkeys.size() && selected != nullptr; i++)
                    {
                        dist = glm::length(monkeys[i]->get_pos() - mouse);
                        if(dist < monkeys[i]->size + 4 && dist < min_dist)
                        {
                            min_dist = dist;
                            aux = monkeys[i];
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

	if(ImGui::Button("START ROUND"))
		this->map_layout->start_round();

    //DEBUG

    ImGui::Text("\n\n");
    if(ImGui::Button("DEBUG"))
        debug_active = !debug_active;

    if(debug_active)
    {

        /*
		ImGui::SliderInt("brate", &bloon_rate, 1, 60);
        ImGui::SliderInt("blayers", &bloon_layers, 1, 100);
        ImGui::SliderFloat("bspeed", &bloon_speed, 1, 1000);
		*/
		ImGui::SliderInt("Round", &new_round, 1, 60);
		if(ImGui::Button("Override Round"))
		{
			this->map_layout->override_round(new_round);
		}

		if(ImGui::Button("clear bloons"))
        {
            while(!this->bloons.empty())
                bloons.erase(bloons.begin());
        }

        if(ImGui::Button("clear monkeys"))
        {
            while(!this->monkeys.empty())
                monkeys.erase(monkeys.begin());
            map_layout->reset_placing_map();
        }

        if(ImGui::Button("add 100$"))
        {
            money += 100;
        }

        if(ImGui::Button("add 1000$"))
        {
            money += 1000;
        }

		text = "Bloons on sceen: " + std::to_string(bloons.size());
		ImGui::Text(text.c_str());

		text = "Monkeys on sceen: " + std::to_string(monkeys.size());
		ImGui::Text(text.c_str());

		text = "Projectiles on sceen: " + std::to_string(projectiles.size());
		ImGui::Text(text.c_str());
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

    for(int i = 0; i < monkeys.size(); i++)
        monkeys[i]->draw(sRenderer);

    drawGUI();

    glfwSwapBuffers(window);
}

void Game::cleanup()
{
    for(int i = 0; i < monkeys.size(); i++)
    {
        if(!monkeys[i]->exists())
        {
            monkeys.erase(monkeys.begin() + i);
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
            if(bloons[i]->get_layers() > 0)
                health -= bloons[i]->get_layers();
            bloons.erase(bloons.begin() + i);
            i--;
        }
    }
}
