#include <tonc.h>
#include "metr.h"  //metroid object
#include "samusrunR.h"
#include "Monster.h" //monster class
#include "brin.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RGB16(r,g,b) ((r)+(g<<5)+(b<<10))
//Screen is [10+15*240] RGB16(0,31,0)
#define MAPX 192;
#define MAPY 64;
OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;
int offset = 64;
int samx = 48, samy = 104; //samus starting pos
int mapx = MAPX;
int mapy = MAPY; //map position

void initMain(){
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
    return;
}

void title_Screen(){
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
    tte_init_se_default(0, BG_CBB(0)|BG_SBB(31));
    tte_write("#{P:72,135}");
    tte_write("Press start!");
    tte_write("#{P:72,15}");
    tte_write("Metroid Chase");
    while (1){
        vid_vsync();
        key_poll();
        if(key_hit(KEY_START))
            break;
    }
}

void loadBG(){
    memcpy(pal_bg_mem, brinPal, brinPalLen);
    memcpy(&tile_mem[0][0], brinTiles, brinTilesLen);
    memcpy(&se_mem[30][0], brinMap, brinMapLen);
    REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;
}

void check_input(bool &jumping, int &screenSpeed){
    key_poll();
    
    if (key_tri_horz() > 0)
        samx += 3*key_tri_horz()-2*screenSpeed; //samus move
    else if (key_tri_horz() < 0)
        samx += 1*key_tri_horz()-2*screenSpeed;
    else
        samx += -screenSpeed;
    
    if (screenSpeed == 0 && key_tri_horz() >0)
        screenSpeed++; //start the screen scrolling
    mapx += 2*screenSpeed;
    if(key_hit(KEY_A)&&!jumping){
        jumping = !jumping;
        samy -= 2;
    }
    else if (jumping && key_held(KEY_A))
        samy -= 2;
    else if(samy == 104 && jumping) //reset the jumping flag
        jumping = !jumping;
    else if (jumping) //falling
        samy +=2;
    
    //clamp samus position to the screen
    if (samx <0)
        samx = 0;
    else if (samx > 208)
        samx = 208;
}

int main(){
    title_Screen();
    loadBG();
    bool quit =false;
    Monster metroid;
	metroid.initMonster(metrPal, metrPalLen, metrTiles, metrTilesLen);
    metroid.loadtoVram();
    memcpy(&tile_mem[4][offset], samusrunRTiles, samusrunRTilesLen);
	initMain();
	//memcpy(pal_obj_mem, samusrunRPal, samusrunRPalLen);
	oam_init(obj_buffer, 128);
	
	u32 tid = 0, pb = 0;
	OBJ_ATTR *metr = &obj_buffer[0];
	OBJ_ATTR *samu[5];
	samu[0] = &obj_buffer[1];
	samu[1] = &obj_buffer[2];
	samu[2] = &obj_buffer[3];
	samu[3] = &obj_buffer[4];
	samu[4] = &obj_buffer[5];
    do{
        for(int i=0; i<5; i++){
            obj_set_attr(samu[i], ATTR0_WIDE, ATTR1_SIZE_16, ATTR2_PALBANK(pb)|(tid+offset+4*i));
            obj_set_pos(samu[i], samx, samy+(8*i));
        }
        obj_set_attr(metr, ATTR0_SQUARE, ATTR1_SIZE_64, ATTR2_PALBANK(pb)|tid);
        obj_set_pos(metr, metroid.xpos, metroid.ypos);
        int numframe=0;
        int framerate=0;
        int samustile=0;
        int screenSpeed = 0;
        bool jumping = false;
        metroid.xpos = -65;
        metroid.ypos = 64;
        for( ; ;){ //main loop
            vid_vsync();
            check_input(jumping, screenSpeed);
            samustile = numframe*20; //tile start for frame of animation.
            framerate++;
            if(framerate==4){
                numframe++;
                framerate =0;
                if (numframe == 10)
                    numframe = 0;
            }
            metr->attr2 = ATTR2_BUILD(tid, pb, 0);
            obj_set_pos(metr, metroid.xpos, metroid.ypos);
            for(int i=0; i<5; i++){
                samu[i]->attr2 = ATTR2_BUILD(samustile+offset+i*4, pb, 0);
                obj_set_pos(samu[i], samx, samy+i*8);
            }
            REG_BG0HOFS=mapx;
            REG_BG0VOFS=mapy;
            oam_copy(oam_mem, obj_buffer, 6);
        }
        //do stuff after metroid touches you x times
        quit = true;
    }while(quit);
	return 0;
	}			
