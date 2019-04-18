/*
 * mutator.cpp
 *
 *  Created on: 10 квіт. 2019 р.
 *      Author: sergi
 */

#include <../types.h>

#include <iostream>
#include <map>
#include <fstream>

DEVILUTION_BEGIN_NAMESPACE

std::ifstream mutator_file;
std::ofstream mutator_output;

bool altHightlightingEnabled = false;
bool monsterHP = false;
bool playSFXWhenLevelUp = false;
bool runInTown = false;
bool adria_restoreMana = false;
bool pepin_restoreMana = false;
bool pepin_noHeal = false;
bool im_noReturn = false;

std::map<std::string, bool*> mutator_operators;

void init_mutator()
{
	// init the map
	mutator_operators["enableAltHightlighting"] = &altHightlightingEnabled;
	mutator_operators["showMonsterHP"] = &monsterHP;
	mutator_operators["playLevelUpSFX"] = &playSFXWhenLevelUp;
	mutator_operators["runInTown"] = &runInTown;
	mutator_operators["adria_restoreMana"] = &adria_restoreMana;
	mutator_operators["pepin_restoreMana"] = &pepin_restoreMana;
	mutator_operators["pepin_noHeal"] = &pepin_noHeal;
	mutator_operators["im_noReturn"] = &im_noReturn;

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

	mutator_file.close();
}

bool create_mutator_file()
{
	mutator_output.open("mutator.config");
	if(!mutator_output)
		return false;

	mutator_output << "enableAltHightlighting" << std::endl;
	mutator_output.flush();
	mutator_output << "showMonsterHP" << std::endl;
	mutator_output.flush();
	mutator_output << "playLevelUpSFX" << std::endl;
	mutator_output.flush();
	mutator_output << "runInTown";
	mutator_output.flush();

	mutator_output.close();
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
}

DEVILUTION_END_NAMESPACE
