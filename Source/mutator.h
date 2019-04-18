/*
 * mutator.h
 *
 *  Created on: 10 квіт. 2019 р.
 *      Author: sergi
 */

#ifndef SOURCE_MUTATOR_H_
#define SOURCE_MUTATOR_H_

extern bool altHightlightingEnabled;
extern bool monsterHP;
extern bool playSFXWhenLevelUp;
extern bool runInTown;
extern bool adria_restoreMana;
extern bool pepin_restoreMana;
extern bool pepin_noHeal;
extern bool im_noReturn;

void init_mutator();
bool create_mutator_file();
void read_mutator_file();

#endif /* SOURCE_MUTATOR_H_ */
