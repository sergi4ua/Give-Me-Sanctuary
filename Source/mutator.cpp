/*
 * mutator.cpp
 *
 *  Created on: 10 квіт. 2019 р.
 *      Author: sergi
 */

#include <../types.h>

#include <iostream>
#include <vector>
#include <map>
#include <fstream>

DEVILUTION_BEGIN_NAMESPACE

std::ifstream mutator_file;
std::ofstream mutator_output;
std::string devnull;

bool altHightlightingEnabled = false;
bool monsterHP = false;

std::map<std::string, bool*> mutator_operators;

void init_mutator()
{
	// init the map
	mutator_operators["enableAltHightlighting"] = &altHightlightingEnabled;
	mutator_operators["showMonsterHP"] = &monsterHP;

	// init the mutator
	mutator_file.open("mutator.config", std::ios_base::out);

	// if the config file doesn't exist, recreate it
	if(!mutator_file)
	{
		MessageBoxA(NULL, "Warning: failed to open mutator config file. File will be recreated.", "Mutator", 1);
		if(!create_mutator_file())
		{
			MessageBoxA(NULL, "Warning: failed to create mutator file. Default values will be used.", "Mutator", 1);
		}
		else
		{
			init_mutator();
		}
		mutator_file.close();
	}
	else
		read_mutator_file();
}

bool create_mutator_file()
{
	mutator_output.open("mutator.config");
	if(!mutator_output)
		return false;

	mutator_output << "enableAltHightlighting" << std::endl;
	mutator_output.flush();
	mutator_output << "showMonsterHP";
	mutator_output.flush();

	mutator_file.close();
	return true;
}

constexpr unsigned int string2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (string2int(str, h+1)*33) ^ str[h];
}

void read_mutator_file()
{
	// the mutator config should be already open.
	// read the values :)
	std::string fileline;

	while(mutator_file >> fileline)
	{
		if(fileline[0] == '#') // comments
		     continue;

		if(mutator_operators.count(fileline)){
			*mutator_operators[fileline] = true;
		}
	}

	mutator_file.close();
}

DEVILUTION_END_NAMESPACE
