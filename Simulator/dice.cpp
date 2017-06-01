#include "dice.h"

Dice::Dice()
{
	srand(clock());
}

bool Dice::addLot(int tag, int chance)
{
	if(pool.find(tag) != pool.end())
		return false;
	if(tag == -1)
		return false;
	pool[tag] = chance;
	sumChance += chance;
	return true;
}

bool Dice::removeLot(int tag)
{
	if(pool.find(tag) == pool.end())
		return false;
	sumChance -= pool[tag];
	pool.erase(tag);
}


std::vector<Dice::Lot>Dice::allLots()
{
	std::vector<Lot> result;
	for(std::pair<int, int> p : pool)
	{
		result.push_back(Lot{p.first,p.second});
	}
	return result;
}

void Dice::clear()
{
	sumChance = 0;
	pool.clear();
}

int Dice::next()
{
	int r = rand()%sumChance;
	int current = 0;
	for(std::pair<int, int> p : pool)
	{
		current += p.second;
		if(r < current)
			return p.first;
	}
	return -1;
}

