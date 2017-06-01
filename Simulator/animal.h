#pragma once

#include "panel.h"
#include <unordered_map>
#include <windows.h>

typedef class Animal
{
	public:
	virtual void action(PPANEL)=0;

	private:
	std::unordered_map<int,int> attribute;

} ANIMAL, *PANIMAL;
