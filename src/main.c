#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tice.h>
#include <keypadc.h>
#include <fileioc.h>
#include <graphx.h>

#include "gfx/palette_gfx.h"
#include "gfx/gfx.h"

#include "levels.h"


uint8_t car_positions[2][19];


void DrawTitle(){
	gfx_SetTextScale(1,1);
	gfx_SetTextXY(118,10);
	gfx_PrintString("RushCE v1.2.0");
}


void DrawMenu(){//Draw the menu at the bottom
	gfx_PrintStringXY("-100", 17, 230);
	gfx_PrintStringXY("-10", 86, 230);
	gfx_PrintStringXY("Random", 136, 230);
	gfx_PrintStringXY("+10", 213, 230);
	gfx_PrintStringXY("+100", 273, 230);
}


void DrawBoard(){//Draw the black grid
	gfx_SetColor(254);
	uint8_t y, z;
	for(y = 0; y<7; ++y){//Draw horizontal lines
		gfx_Line(64, y * 32 + 32, 256, y * 32 + 32);
	}
	for(z = 0; z<7; ++z){//Draw vertical lines
			gfx_Line(z * 32 + 64, 32, z * 32 + 64, 224);
	}
	gfx_SetColor(235);
	gfx_Line(256,97,256,127);//Draw exit
}


void GetCarPositionsFromId(uint16_t game_id){//Decode one level from levels.h and save the position and rotation of all cars in car_positions
	uint8_t x, y;
	int two_bytes;
	unsigned char encoded_game[15];

	memcpy((char *)encoded_game, levels + 15 * (1810 - game_id), 15);//Copy one level from levels.h to encoded_game

	for(x = 0; x<2; x++){//Clear the board
		for(y = 0; y<18; y++){
			car_positions[x][y] = 0;
		}
	}

	for(x = 0; x<6; x++){//Decode 6 2-byte base 37 numbers from encoded_game and store them in car_positions
		two_bytes = encoded_game[x * 2] * 256 + encoded_game[x * 2 + 1];//Store 2 Bytes from encoded_game in two_bytes
		for(y = 0; y<3; y++){//Convert two_bytes into 3 car positions and store them in car_positions
			car_positions[0][x * 3 + 2 - y] = two_bytes % 37;
			two_bytes = two_bytes / 37;
		}
	}

	for(x = 0; x<8; x++){
		car_positions[1][x] = encoded_game[12] % 2;
		if(car_positions[1][x] == 0 && car_positions[0][x] != 36){car_positions[1][x] = 2;}
		encoded_game[12] = encoded_game[12] / 2;
	}
	for(x = 8; x<16; x++){
		car_positions[1][x] = encoded_game[13] % 2;
		if(car_positions[1][x] == 0 && car_positions[0][x] != 36){car_positions[1][x] = 2;}
		encoded_game[13] = encoded_game[13] / 2;
	}
    car_positions[0][18] = encoded_game[14];//Store the mininum number of moves for the level in car_positions[0][14]
	//Format of car_positions:
	//[[Position],[Rotation]]
	//Rotation 1 = horizontal, Rotation 2 = vertical
}


void DrawLevelInfo(uint16_t game_id, uint16_t moves, uint16_t highscore){
	//Left side
	gfx_SetColor(253);
	gfx_FillRectangle(10,30,40,40);

	gfx_PrintStringXY("Level:", 10, 30);
	gfx_SetTextXY(10,40);
	gfx_PrintInt(game_id + 1, 1);
	gfx_PrintStringXY("/", 42, 40);
	gfx_PrintStringXY("1811", 10, 50);

	//Highscore
	gfx_FillRectangle(240,10,75,20);
	if(highscore > 0){
		gfx_PrintStringXY("Highscore:", 240, 10);
		gfx_SetTextXY(240,20);
		gfx_PrintInt(highscore, 1);
	}

	//Right side
	gfx_FillRectangle(267,30,40,40);
	gfx_PrintStringXY("Moves:", 267, 30);
	gfx_SetTextXY(267,40);
    gfx_PrintInt(moves, 1);
	gfx_PrintStringXY("/", 283, 40);
	gfx_SetTextXY(267,50);
	gfx_PrintInt((int)car_positions[0][18], 1);
}


void DrawCars(){
	uint8_t x, height, width;
	for(x = 0; x<16; x++){
		if(car_positions[1][x] == 0){continue;}//No car
		if(car_positions[1][x] == 2){height = 27; width = 59;}//Short car vertical
		if(car_positions[1][x] == 2 && x < 5 && x > 0){height = 27; width = 91;}//Long car vertical
		if(car_positions[1][x] == 1){height = 59; width = 27;}//Short car horizontal
		if(car_positions[1][x] == 1 && x < 5 && x > 0){height = 91; width = 27;}//Long car horizontal
		gfx_SetColor(252 - x);
		gfx_FillRectangle(67 + 32 * (car_positions[0][x] % 6), 32 * (car_positions[0][x] / 6) + 35, height, width);//Draw red car
	}
	if(car_positions[0][16] != 36){
		gfx_SetColor(236);
		gfx_FillRectangle(67 + 32 * (car_positions[0][16] % 6), 32 * (car_positions[0][16] / 6) + 35, 27, 27);//Draw first wall
	}
	if(car_positions[0][17] != 36){
		gfx_SetColor(236);
		gfx_FillRectangle(67 + 32 * (car_positions[0][17] % 6), 32 * (car_positions[0][17] / 6) + 35, 27, 27);//Draw second wall
	}
}

void DrawWinScreen(){
	gfx_FillScreen(253);
	gfx_PrintStringXY("Congratulations! You have completed the level!", 2, 100);
	gfx_PrintStringXY("Press 2nd to return to the main menu...", 37, 120);
}


int CheckPos(uint8_t pos){//Check, which car occupies a position
	uint8_t x, car;
	car = 99;
	for(x = 0; x < 16; x++){
		if(car_positions[0][x] == pos && car_positions[1][x] != 0){car = x; break;}
	}
    if(car_positions[0][16] == pos){car = 16;}
    if(car_positions[0][17] == pos){car = 17;}
	for(x = 0; x < 16; x++){
		if(car == 99 && car_positions[1][x] == 1 && car_positions[0][x] == pos - 1 && car_positions[1][x] != 0){car = x; break;}
	}
	for(x = 0; x < 16; x++){
		if(car == 99 && car_positions[1][x] == 2 && car_positions[0][x] == pos - 6 && car_positions[1][x] != 0){car = x; break;}
	}
	for(x = 1; x < 5; x++){
		if(car == 99 && car_positions[1][x] == 1 && car_positions[0][x] == pos - 2 && car_positions[1][x] != 0){car = x; break;}
	}
	for(x = 1; x < 5; x++){
		if(car == 99 && car_positions[1][x] == 2 && car_positions[0][x] == pos - 12 && car_positions[1][x] != 0){car = x; break;}
	}
	return car;
}


void DrawCursor(uint8_t pos, uint8_t color){
	uint8_t x, car;

	car = CheckPos(pos);//Check, which car is at position
	if(car == 99){//Draw the cursor just at pos if no car is at position
		gfx_SetColor(252 - color);
		gfx_FillRectangle(pos % 6 * 32 + 64, pos / 6 * 32 + 32, 32, 32);
		gfx_SetColor(253);
		gfx_FillRectangle(pos % 6 * 32 + 67, pos / 6 * 32 + 35, 27, 27);
	}
	else{
		for(x = 0; x < 36; x++){//Draw cursor at any position where a part of the car, which at pos is, is
			if(CheckPos(x) == car){
				gfx_SetColor(252 - color);
				gfx_FillRectangle(x % 6 * 32 + 64, x / 6 * 32 + 32, 32, 32);
			}
		}
	}
}



int main(void){

	uint16_t game_id, keycountC, keycountA, cursorpos, selectedcar, moves, latestcar, game_status;
	kb_key_t keyA, keyC, prevkeyA, prevkeyC;
	uint8_t highscores[1813] = {0};
	uint16_t* highscore_sv = (uint16_t*)&highscores;

	srandom(rtc_Time());
	game_id = 0;
	cursorpos = 0;
	moves = 0;
	selectedcar = 98;
	latestcar = 99;
	game_status = 0;

	//Load appvar
	ti_var_t sv = ti_Open("RUSHSV","r+");
	if(ti_GetSize(sv) < 1000){//Create appvar if it doesn't exist
		ti_Write(highscore_sv,1813,1,sv);
		ti_SetArchiveStatus(true, sv);
	}
	ti_Read(highscore_sv,1813,1,sv);
	ti_Close(sv);

	game_id = highscores[1811] * 256 + highscores[1812];

	gfx_Begin();
    gfx_SetPalette(palette_gfx, sizeof_palette_gfx, 0);
	gfx_SetDrawBuffer();

	gfx_FillScreen(253);
	DrawTitle();
	gfx_SwapDraw();

	gfx_FillScreen(253);
	DrawTitle();

	while (!(kb_Data[6] == kb_Clear)){//Main loop

		//Controls
		if(kb_Data[7] == kb_Left && keyC != prevkeyC){//Left
			if(game_status == 0 && game_id > 0){game_id = game_id - 1;}
			if(game_status == 1 && cursorpos % 6 > 0 && selectedcar == 98){//Move cursor left
				if(CheckPos(cursorpos - 1) == 99){cursorpos = cursorpos - 1;}
				else if(CheckPos(cursorpos) != CheckPos(cursorpos - 1)){cursorpos = car_positions[0][CheckPos(cursorpos - 1)];}
				else if(CheckPos(cursorpos - 2) == 99 && cursorpos % 6 > 1){cursorpos = cursorpos - 2;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos - 1 ) && CheckPos(cursorpos) != CheckPos(cursorpos - 2) && cursorpos % 6 > 1){cursorpos = car_positions[0][CheckPos(cursorpos - 2)];}
				else if(CheckPos(cursorpos - 3) == 99 && cursorpos % 6 > 2){cursorpos = cursorpos - 3;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos - 1 ) && CheckPos(cursorpos) == CheckPos(cursorpos - 2) && cursorpos % 6 > 2){cursorpos = car_positions[0][CheckPos(cursorpos - 3)];}
			}
			if(game_status == 1 && cursorpos % 6 > 0 && selectedcar != 98 && CheckPos(car_positions[0][CheckPos(cursorpos)] - 1) == 99 && car_positions[1][CheckPos(cursorpos)] == 1){//Move car left
				car_positions[0][CheckPos(cursorpos)] = car_positions[0][CheckPos(cursorpos)] - 1;
				cursorpos = cursorpos - 1;
				if(CheckPos(cursorpos) != latestcar){
					moves = moves + 1;
				}
				latestcar = CheckPos(cursorpos);
			}
		}

		if(kb_Data[7] == kb_Right && keyC != prevkeyC){//Right
			if(game_status == 0 && game_id < 1810){game_id = game_id + 1;}
			if(game_status == 1 && cursorpos % 6 < 5 && selectedcar == 98){//Move cursor right
				if(CheckPos(cursorpos + 1) == 99){cursorpos = cursorpos + 1;}
				else if(CheckPos(cursorpos) != CheckPos(cursorpos + 1)){cursorpos = car_positions[0][CheckPos(cursorpos + 1)];}
				else if(CheckPos(cursorpos + 2) == 99 && cursorpos % 6 < 4){cursorpos = cursorpos + 2;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos + 1) && CheckPos(cursorpos) != CheckPos(cursorpos + 2) && cursorpos % 6 < 4){cursorpos = car_positions[0][CheckPos(cursorpos + 2)];}
				else if(CheckPos(cursorpos + 3) == 99 && cursorpos % 64 < 3){cursorpos = cursorpos + 3;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos + 1 ) && CheckPos(cursorpos) == CheckPos(cursorpos + 2) && cursorpos % 6 < 3){cursorpos = car_positions[0][CheckPos(cursorpos + 3)];}
			}
			if(game_status == 1 && selectedcar != 98 && (((CheckPos(cursorpos) == 0 || (CheckPos(cursorpos) > 4  && CheckPos(cursorpos) < 16)) && CheckPos(car_positions[0][CheckPos(cursorpos)] + 2) == 99 && cursorpos % 6 < 4)|| ((CheckPos(cursorpos) > 0  && CheckPos(cursorpos) < 5) && CheckPos(car_positions[0][CheckPos(cursorpos)] + 3) == 99 && cursorpos % 6 < 3)) && car_positions[1][CheckPos(cursorpos)] == 1){//Move car right
				car_positions[0][CheckPos(cursorpos)] = car_positions[0][CheckPos(cursorpos)] + 1;
				cursorpos = cursorpos + 1;
				if(CheckPos(cursorpos) != latestcar){
					moves = moves + 1;
				}
				latestcar = CheckPos(cursorpos);
				if(car_positions[0][0] == 16){//Win
					highscores[game_id] = moves;
					game_status = 2;
					cursorpos = 0;
					selectedcar = 98;
					moves = 0;
					latestcar = 99;
				}
			}
		}

		if(kb_Data[7] == kb_Up && keyC != prevkeyC){//Up
			if(game_status == 1 && cursorpos / 6 > 0 && selectedcar == 98){//Move cursor up
				if(CheckPos(cursorpos - 6) == 99){cursorpos = cursorpos - 6;}
				else if(CheckPos(cursorpos) != CheckPos(cursorpos - 6)){cursorpos = car_positions[0][CheckPos(cursorpos - 6)];}
				else if(CheckPos(cursorpos - 12) == 99 && cursorpos / 6 > 1){cursorpos = cursorpos - 12;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos - 6 ) && CheckPos(cursorpos) != CheckPos(cursorpos - 12) && cursorpos / 6 > 1){cursorpos = car_positions[0][CheckPos(cursorpos - 12)];}
				else if(CheckPos(cursorpos - 18) == 99 && cursorpos / 6 > 2){cursorpos = cursorpos - 18;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos - 6 ) && CheckPos(cursorpos) == CheckPos(cursorpos - 12) && cursorpos / 6 > 2){cursorpos = car_positions[0][CheckPos(cursorpos - 18)];}
			}
			if(game_status == 1 && cursorpos / 6 > 0 && selectedcar != 98 && CheckPos(car_positions[0][CheckPos(cursorpos)] - 6) == 99 && car_positions[1][CheckPos(cursorpos)] == 2){//Move car up
				car_positions[0][CheckPos(cursorpos)] = car_positions[0][CheckPos(cursorpos)] - 6;
				cursorpos = cursorpos - 6;
				if(CheckPos(cursorpos) != latestcar){
					moves = moves + 1;
				}
				latestcar = CheckPos(cursorpos);
			}
		}

		if(kb_Data[7] == kb_Down && keyC != prevkeyC){//Down
			if(game_status == 1 && cursorpos / 6 < 5 && selectedcar == 98){//Move cursor down
				if(CheckPos(cursorpos + 6) == 99){cursorpos = cursorpos + 6;}
				else if(CheckPos(cursorpos) != CheckPos(cursorpos + 6)){cursorpos = car_positions[0][CheckPos(cursorpos + 6)];}
				else if(CheckPos(cursorpos + 12) == 99 && cursorpos / 6 < 4){cursorpos = cursorpos + 12;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos + 6 ) && CheckPos(cursorpos) != CheckPos(cursorpos + 12) && cursorpos / 6 < 4){cursorpos = car_positions[0][CheckPos(cursorpos + 12)];}
				else if(CheckPos(cursorpos + 18) == 99 && cursorpos / 6 < 3){cursorpos = cursorpos + 18;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos + 6 ) && CheckPos(cursorpos) == CheckPos(cursorpos + 12) && cursorpos / 6 < 3){cursorpos = car_positions[0][CheckPos(cursorpos + 18)];}
			}

			if(game_status == 1 && selectedcar != 98 && ((((CheckPos(cursorpos) > 4  && CheckPos(cursorpos) < 16)) && CheckPos(car_positions[0][CheckPos(cursorpos)] + 12) == 99  && cursorpos / 6 < 4) || ((CheckPos(cursorpos) > 0  && CheckPos(cursorpos) < 5) && CheckPos(car_positions[0][CheckPos(cursorpos)] + 18) == 99 && cursorpos / 6 < 3)) && car_positions[1][CheckPos(cursorpos)] == 2){//Move car down
				car_positions[0][CheckPos(cursorpos)] = car_positions[0][CheckPos(cursorpos)] + 6;
				cursorpos = cursorpos + 6;
				if(CheckPos(cursorpos) != latestcar){
					moves = moves + 1;
				}
				latestcar = CheckPos(cursorpos);
			}
		}

		if(kb_Data[1] == kb_2nd && keyA != prevkeyA){//2nd
		 if (game_status == 0){game_status = 1;}
		 else if (game_status == 1 && CheckPos(cursorpos) != 99 && CheckPos(cursorpos) != 17 && CheckPos(cursorpos) != 16 && selectedcar == 98){//select car to move
			 selectedcar = CheckPos(cursorpos);
		 }
		 else if (game_status == 1 && selectedcar != 98){//unselect car
			 selectedcar = 98;
			 latestcar = 99;
		 }
		 if(game_status == 2){//Exit the win screen
			 game_status = 0;
			 cursorpos = 0;
			 selectedcar = 98;
			 moves = 0;
			 latestcar = 99;
			 gfx_FillScreen(253);
			 DrawTitle();
			 gfx_SwapDraw();
			 gfx_FillScreen(253);
			 DrawTitle();
		 }
		}

		if(kb_Data[1] == kb_Del && keyA != prevkeyA){//del
			if(game_status == 0){//Exit the game
				break;
			}
			if(game_status == 1){//Exit the level
				game_status = 0;
				cursorpos = 0;
				selectedcar = 98;
				moves = 0;
				latestcar = 99;
			}
			if(game_status == 2){//Exit the win screen
				game_status = 0;
				cursorpos = 0;
				selectedcar = 98;
				moves = 0;
				latestcar = 99;
				gfx_FillScreen(253);
 			  DrawTitle();
 			  gfx_SwapDraw();
 			  gfx_FillScreen(253);
 			  DrawTitle();
			}
		}

		//Bottom menu navigation
        if((keyA != prevkeyA || keycountA > 2) && game_status == 0){
            if(kb_Data[1] == kb_Yequ && game_id > 99){game_id = game_id - 100;}
  	    	if(kb_Data[1] == kb_Yequ && game_id < 100){game_id = 0;}
  	    	if(kb_Data[1] == kb_Window && game_id > 9){game_id = game_id - 10;}
  	    	if(kb_Data[1] == kb_Window && game_id < 10){game_id = 0;}
  	    	if(kb_Data[1] == kb_Zoom){game_id = random() % 1811;}
  	    	if(kb_Data[1] == kb_Trace && game_id < 1801){game_id = game_id + 10;}
  	    	if(kb_Data[1] == kb_Trace && game_id > 1800){game_id = 1810;}
  	    	if(kb_Data[1] == kb_Graph && game_id < 1711){game_id = game_id + 100;}
  	    	if(kb_Data[1] == kb_Graph && game_id > 1710){game_id = 1810;}
        }
	      
  
		if(game_status == 0){GetCarPositionsFromId(game_id);}


		//Graphics
		gfx_SetColor(253);
		gfx_FillRectangle(64,32,193,193);//Redraw Background
		if(game_status == 1 && selectedcar == 98){DrawCursor(cursorpos, 0);}
		if(game_status == 1 && selectedcar != 98){DrawCursor(cursorpos, 3);}
		DrawBoard();
		DrawLevelInfo(game_id, moves, highscores[game_id]);
		gfx_SetColor(253);
		gfx_FillRectangle(10,230,300,8);
		DrawCars();
		if(game_status == 0){DrawMenu();}
		if(game_status == 2){DrawWinScreen();}
		gfx_SwapDraw();

		kb_Scan();

		//Long key press detection
		prevkeyA = keyA;
		prevkeyC = keyC;
		keyC = kb_Data[7];
		keyA = kb_Data[1];

		if(keyC == prevkeyC){
			keycountC = keycountC + 1;
			if(keycountC > 3){
				if(keycountC % 2 == 1){prevkeyC = 0;}
			}
		}
		else{keycountC = 0;}

		if(keyA == prevkeyA){
			keycountA = keycountA + 1;
		}
		else{keycountA = 0;}


	}

	//Save appvar
	highscores[1811] = game_id / 256;
	highscores[1812] = game_id % 256;
	sv = ti_Open("RUSHSV","w");
  ti_Write(highscore_sv,1813,1,sv);
	ti_SetArchiveStatus(true, sv);
  ti_Close(sv);


  pgrm_CleanUp();
  gfx_End();
}
