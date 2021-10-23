#include "RoundGenerator.hpp"

RoundGenerator::RoundGenerator()
{

}

RoundGenerator::RoundGenerator(std::string rounds_path)
{
	std::ifstream ifs(rounds_path);
	ifs >> this->rounds_json;
	round = -1; //-1 because at the start we add 1 and we get 0
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

void RoundGenerator::read_round()
{
	Json::Value current_round;
	if(this->round < rounds_json["rounds"].size())
	{
		current_round = rounds_json["rounds"][this->round];
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
		current_round = rounds_json["auto"];
		int type;
		for(int i = 0; i < current_round.size(); i++)
		{
			type = current_round[i]["type"].asInt();
			if(type < this->bloons_left.size())
			{
				this->bloons_left[type].b_amount = 14;
				this->bloons_left[type].b_speed += current_round[i]["increment"].asDouble();
				this->bloons_left[type].b_freq += current_round[i]["increment"].asDouble();
				this->bloons_left[type].timer = -current_round[i]["time_offset"].asDouble();
			}
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
				std::cout << elem->second.timer << '\n';
				elem->second.timer = 0;
				elem->second.b_amount--;
			}

		}
	}
}

void RoundGenerator::bloon_popped(Bloon bloon)
{
	if(this->bloons_left.find(bloon.get_type()) != this->bloons_left.end())
	{
		this->bloons_left[bloon.get_type()].b_amount--;
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
