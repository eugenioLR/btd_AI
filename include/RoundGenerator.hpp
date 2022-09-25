#pragma once

#include "Tower.hpp"
#include <map>

struct bloon_info_t
{
	int b_amount;
	double b_freq;
	double b_speed;
	double timer;
};

class RoundGenerator
{
private:
	Json::Value rounds_json;
	int round;
	std::map<int, bloon_info_t> bloons_to_place;
	std::map<int, bloon_info_t> bloons_left;
	std::map<int, bloon_info_t> auto_base;
	int auto_reward;
	double time;
public:
	RoundGenerator();
	RoundGenerator(std::string);
	int get_round();
	void start_round();
	void override_round(int);
	void read_round();
	void update(double, std::vector<Bloon*>*, std::queue<glm::vec2>);
	void bloon_popped(Bloon, int*);
	bool round_finished();
};
