#pragma once
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

typedef class Dice
{
	public:
	Dice();
	
	struct Lot
	{
		int tag;
		int chance;
	};

	//	add a tag into the lot pool
	//	tag: index of tag, chance: the num of tag add into the pool
	//	true: success, false: tag duplicate
	bool addLot(int tag, int chance);

	//	remove a tag
	//	tag: index of the tag will be removed
	//	true: tag remove, false: tag not found
	bool removeLot(int tag);
	
	//	clear all tags
	void clear();

	//	all lots in the pool
	std::vector<Lot> allLots();

	//	return the next random tag selected
	int next();

	private:
	std::unordered_map<int,int> pool;
	int sumChance = 0;

} DICE, *PDICE;
