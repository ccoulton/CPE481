#include <tonc.h>
#include "metr.h"  //metroid object
#include "Monster.h" //moster class

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CLR_UP RGB15(27,27,29)
#define RGB16(r,g,b) ((r)+(g<<5)+(b<<10))
//Screen is [10+15*240] RGB16(0,31,0)
OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;

void initMain(){
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	//REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_WIN0;
	return;
}
int main(){
	Monster metroid;
	metroid.initMonster(metrPal, metrPalLen, metrTiles, metrTilesLen);
	initMain();
	metroid.loadtoVram();
	oam_init(obj_buffer, 128);
	REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D;
	metroid.xpos = 96;
	metroid.ypos = 32;
	u32 tid = 0, pb = 0;
	OBJ_ATTR *metr = &obj_buffer[0];
	obj_set_attr(metr, 
		ATTR0_SQUARE,
		ATTR1_SIZE_64,
		ATTR2_PALBANK(pb)|tid);
	//obj_set_pos(samus, samus.xpos, samus.ypos);
	obj_set_pos(metr, metroid.xpos, metroid.ypos);
	
	for( ; ;){
		vid_vsync();
		key_poll();
		metroid.xpos += 2*key_tri_horz();
		metroid.ypos += 2*key_tri_vert();
		tid += bit_tribool(key_hit(-1), KI_R, KI_L);
		
		if(key_hit(KEY_START))
			REG_DISPCNT ^= DCNT_OBJ_1D;
		
		metr->attr2 = ATTR2_BUILD(tid, pb, 0);
		obj_set_pos(metr, metroid.xpos, metroid.ypos);
		oam_copy(oam_mem, obj_buffer, 1);
	}
	return 0;
	}			