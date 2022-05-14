#include "RoundGenerator.hpp"

RoundGenerator::RoundGenerator()
{

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

	this->round = -1; //-1 because at the start we add 1 and we get 0
}

int RoundGenerator::get_round()
{
	return this->round;
}

void RoundGenerator::start_round()
{

	if(this->round_finished())
	{
		round++;
		std::cout << "ROUND " << round << " started" << '\n';

		this->read_round();
	}
}

void RoundGenerator::override_round(int new_round)
{

	if(this->round_finished())
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
		for(int i = 0; i < increments.size(); i++)
		{
			aux_info = auto_base[i];
			aux_info.b_amount += increments[i]["number"].asInt();
			aux_info.b_freq += increments[i]["freq"].asDouble();
			this->bloons_left[increments[i]["type"].asInt()] = aux_info;
			this->bloons_to_place[increments[i]["type"].asInt()] = aux_info;
		}
	}


}

void RoundGenerator::update(double deltatime, std::vector<Bloon*>* bloon_list, std::queue<glm::vec2> bloon_path)
{
	Bloon* bloon;
	if(!this->round_finished())
	{
		std::map<int, bloon_info_t>::iterator elem;
       	for (elem = this->bloons_to_place.begin(); elem != this->bloons_to_place.end(); elem++)
		{
			elem->second.timer += deltatime;
			if(elem->second.timer > 1/elem->second.b_freq && elem->second.b_amount > 0)
			{
				bloon = new Bloon(bloon_path, elem->first, elem->second.b_speed);
			    bloon_list->push_back(bloon);
				elem->second.timer = 0;
				elem->second.b_amount--;
			}

		}
	}
}

void RoundGenerator::bloon_popped(Bloon bloon, int* money)
{
	if(this->bloons_left.find(bloon.get_type()) != this->bloons_left.end())
	{
		this->bloons_left[bloon.get_type()].b_amount--;
	}

	if(this->bloons_left[bloon.get_type()].b_amount == 0)
	{
		*money += rounds_json["rounds"][this->round]["desc"]["reward"].asInt();
	}
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
