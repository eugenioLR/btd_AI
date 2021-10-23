#pragma once

#include "Monkey.hpp"
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
	double time;
public:
	RoundGenerator();
	RoundGenerator(std::string);
	void start_round();
	void read_round();
	void update(double, std::vector<Bloon*>*, std::queue<glm::vec2>);
	void bloon_popped(Bloon);
	bool round_finished();
};
