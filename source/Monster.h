#ifndef __MONSTER__
#define __MONSTER__

class Monster{
	public:
		void initMonster(const unsigned int* Pal,
			int PalLen, 
			const unsigned int* Tiles, 
			int Tilelen); 
		void loadtoVram();
		int xpos, ypos;
	private:
		int tileLen, palLen;
		unsigned int* pallet;
		unsigned int* tiles;
};

#endif