#include "Monster.h"
#include <string.h>
#include <tonc.h>

void Monster::initMonster(const unsigned int* Pal, int PalLen,
	const unsigned int* Tiles, int Tilelen){
	tileLen = Tilelen;
	palLen = PalLen;
	pallet = (unsigned int*) Pal;
	tiles = (unsigned int*) Tiles;
	}
	
void Monster::loadtoVram(){
	memcpy(&tile_mem[4][0], tiles, tileLen); //load bitmap
	memcpy(pal_obj_mem, pallet, palLen); //load palet
	}