#include <tonc.h>
//#include "include/tonc_input.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RGB16(r,g,b) ((r)+(g<<5)+(b<<10))
//Screen is [10+15*240] RGB16(0,31,0)

//volatile int* KEYS = (int*)0x4000130;
unsigned short* Screen = (unsigned short*)0x6000000;

void DrawLine(char xpos);
void initMain(){
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | 
		DCNT_OBJ_1D | DCNT_WIN0;
	return;
}
int main(){
	initMain();
	
	
	for( ; ;){
		VBlankIntrWait();
		key_poll();
	}
	return 0;
	}			