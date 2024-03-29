#include "RoundGenerator.hpp"

RoundGenerator::RoundGenerator()
{
	this->round_running = false;
}

RoundGenerator::RoundGenerator(std::string rounds_path)
{
	std::ifstream ifs(rounds_path);
	ifs >> this->rounds_json;

	auto_reward = rounds_json["auto"]["desc"]["reward"].asInt();
	Json::Value auto_round_base = rounds_json["auto"]["base"]["wave"];
	bloon_info_t aux_info;
	for(int i = 0; i < auto_round_base.size(); i++)
	{
		aux_info.b_amount = auto_round_base[i]["number"].asInt();// + round;
		aux_info.b_freq = auto_round_base[i]["freq"].asDouble();// + ((double)round/100.0);
		aux_info.b_speed = 150;
		aux_info.timer = -auto_round_base[i]["time_offset"].asDouble();
		this->auto_base[auto_round_base[i]["type"].asInt()] = aux_info;
	}

	this->round = 0;
}

int RoundGenerator::get_round()
{
	return this->round;
}

bool RoundGenerator::is_round_running()
{
	return this->round_running;
}

void RoundGenerator::start_round()
{
	if(!this->round_running)
	{
		this->round_running = true;

		std::cout << "ROUND " << round << " started" << '\n';

		this->read_round();
	}
}

void RoundGenerator::override_round(int new_round)
{

	if(!this->round_running)
	{
		round = new_round;
		std::cout << "ROUND " << round << " started" << '\n';

		this->read_round();
	}
}

void RoundGenerator::read_round()
{
	if(this->round < rounds_json["rounds"].size())
	{
		Json::Value current_round = rounds_json["rounds"][this->round]["wave"];
		bloon_info_t aux_info;

		for(int i = 0; i < 6; i++)
		{
			aux_info = auto_base[i];
			aux_info.b_amount = 0;
			aux_info.b_freq = 0;
			aux_info.b_speed = 150;
			aux_info.timer = 0;
			this->bloons_left[i] = aux_info;
			this->bloons_to_place[i] = aux_info;
		}


		for(int i = 0; i < current_round.size(); i++)
		{
			aux_info.b_amount = current_round[i]["number"].asInt();
			aux_info.b_freq = current_round[i]["freq"].asDouble();
			aux_info.b_speed = 150;
			aux_info.timer = -current_round[i]["time_offset"].asDouble();
			this->bloons_left[current_round[i]["type"].asInt()] = aux_info;
			this->bloons_to_place[current_round[i]["type"].asInt()] = aux_info;
		}
	}
	else
	{
		std::cout << "AUTO ROUND" << '\n';
		Json::Value increments = rounds_json["auto"]["increments"]["wave"];
		bloon_info_t aux_info;
		int round_offset = this->round - rounds_json["rounds"].size();

		for(int i = 0; i < 6; i++)
		{
			aux_info = auto_base[i];
			aux_info.b_amount = 0;
			aux_info.b_freq = 1;
			this->bloons_left[increments[i]["type"].asInt()] = aux_info;
			this->bloons_to_place[increments[i]["type"].asInt()] = aux_info;
		}


		for(int i = 0; i < increments.size(); i++)
		{
			aux_info = auto_base[i];
			aux_info.b_amount = round_offset * increments[i]["number"].asInt() + auto_base[i].b_amount;
			aux_info.b_freq = round_offset * increments[i]["freq"].asDouble() + auto_base[i].b_freq;
			this->bloons_left[increments[i]["type"].asInt()] = aux_info;
			this->bloons_to_place[increments[i]["type"].asInt()] = aux_info;
		}
	}
}

void RoundGenerator::update(double deltatime, std::vector<Bloon*>* bloon_list, std::queue<glm::vec2> bloon_path)
{
	Bloon* bloon;
	if(this->round_running)
	{
		std::map<int, bloon_info_t>::iterator elem;
       	for (elem = this->bloons_to_place.begin(); elem != this->bloons_to_place.end(); elem++)
		{
			elem->second.timer += deltatime;
			if(elem->second.timer > 1/elem->second.b_freq && elem->second.b_amount > 0)
			{
				bloon = new Bloon(bloon_path, static_cast<BloonType>(elem->first), elem->second.b_speed);
			    bloon_list->push_back(bloon);
				elem->second.timer = 0;
				elem->second.b_amount--;
			}

		}

		this->round_running = !this->round_finished();
		if(this->round_finished())
			round++;
	}
}

void RoundGenerator::bloon_popped(Bloon bloon, int* money)
{
	if(this->bloons_left.find(bloon.get_type()) != this->bloons_left.end())
	{
		if(this->bloons_left[bloon.get_type()].b_amount > 0)
		{
			this->bloons_left[bloon.get_type()].b_amount--;
		}
	}

	if(this->bloons_left[bloon.get_type()].b_amount == 0)
	{
		*money += rounds_json["rounds"][this->round]["desc"]["reward"].asInt();
	}
}

void RoundGenerator::stop_round()
{
	for (int i = 0; i < 6; i++)
	{
		this->bloons_left[i].b_amount = 0;
	}

	for(auto& elem : this->bloons_left)
	{
		elem.second.b_amount = 0;
	}

	this->round_running = false;
}

bool RoundGenerator::round_finished()
{
	bool finished = true;
	for(const auto& elem : this->bloons_left)
	{
		finished = finished && elem.second.b_amount == 0;
	}
	return finished;
}
