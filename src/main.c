#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <tice.h>
#include <keypadc.h>
#include <fileioc.h>
#include <graphx.h>

#include "gfx/palette_gfx.h"
#include "gfx/gfx.h"

#include "levels.h"


uint8_t block_positions[2][19];


void DrawTitle(){//Draw the title
	gfx_SetTextScale(1,1);
	gfx_SetTextXY(118,10);
	gfx_PrintString("RushCE v1.0.0");
}


void DrawMenu(){//Draw the menu at the bottom
	gfx_PrintStringXY("-100", 17, 230);
	gfx_PrintStringXY("-10", 86, 230);
	gfx_PrintStringXY("Random", 136, 230);
	gfx_PrintStringXY("+10", 213, 230);
	gfx_PrintStringXY("+100", 273, 230);
}


void DrawBoard(){// Redraw the background and draws the black grid
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


void GetBlockPositionsFromId(uint16_t game_id){//Decode one level from levels.h and save the position and rotation of all blocks in block_positions
	uint8_t x, y;
	int three_bytes;
	unsigned char game_string[36], encoded_game_string[23];

	memcpy((char *)encoded_game_string, levels + 23 * (1810 - game_id), 23);//Copy one level from levels.h to encoded_game_string

	for(x = 0; x<2; x++){//Clear the board
		for(y = 0; y<18; y++){
			block_positions[x][y] = 0;
		}
	}

	for(x = 0; x<7; x++){//Encode 7 3-byte base 18 numbers from encoded_game_string to 5 chars in game_string
		three_bytes = encoded_game_string[x * 3] * 65536 + encoded_game_string[x * 3 + 1] * 256 + encoded_game_string[x * 3 + 2];//Store 3 Bytes from encoded_game_string in three_bytes
		for(y = 0; y<5; y++){//Convert three_bytes into 5 chars and store them in game_string
			game_string[x * 5 + 4 - y] = three_bytes % 18 + 'A';
			three_bytes = three_bytes / 18;
		}
	}
	game_string[35] = encoded_game_string[21] + 'A';//Convert last byte of encoded_game_string to char
  block_positions[0][18] = encoded_game_string[22];//Store the mininum number of moves for the level in block_positions[0][18]

		for(x = 0; x<36; x++){//Store position and rotation of all blocks in block_positions
			if (game_string[x] == 'A' && block_positions[1][0] == 0){block_positions[0][0] = x; block_positions[1][0] = 1;}
			if (game_string[x] == 'B' && block_positions[1][1] == 0){block_positions[0][1] = x; if(game_string[x + 1] == 'B'){block_positions[1][1] = 1;}else{block_positions[1][1] = 2;}}
			if (game_string[x] == 'C' && block_positions[1][2] == 0){block_positions[0][2] = x; if(game_string[x + 1] == 'C'){block_positions[1][2] = 1;}else{block_positions[1][2] = 2;}}
			if (game_string[x] == 'D' && block_positions[1][3] == 0){block_positions[0][3] = x; if(game_string[x + 1] == 'D'){block_positions[1][3] = 1;}else{block_positions[1][3] = 2;}}
			if (game_string[x] == 'E' && block_positions[1][4] == 0){block_positions[0][4] = x; if(game_string[x + 1] == 'E'){block_positions[1][4] = 1;}else{block_positions[1][4] = 2;}}
			if (game_string[x] == 'F' && block_positions[1][5] == 0){block_positions[0][5] = x; if(game_string[x + 1] == 'F'){block_positions[1][5] = 1;}else{block_positions[1][5] = 2;}}
			if (game_string[x] == 'G' && block_positions[1][6] == 0){block_positions[0][6] = x; if(game_string[x + 1] == 'G'){block_positions[1][6] = 1;}else{block_positions[1][6] = 2;}}
			if (game_string[x] == 'H' && block_positions[1][7] == 0){block_positions[0][7] = x; if(game_string[x + 1] == 'H'){block_positions[1][7] = 1;}else{block_positions[1][7] = 2;}}
			if (game_string[x] == 'I' && block_positions[1][8] == 0){block_positions[0][8] = x; if(game_string[x + 1] == 'I'){block_positions[1][8] = 1;}else{block_positions[1][8] = 2;}}
			if (game_string[x] == 'J' && block_positions[1][9] == 0){block_positions[0][9] = x; if(game_string[x + 1] == 'J'){block_positions[1][9] = 1;}else{block_positions[1][9] = 2;}}
			if (game_string[x] == 'K' && block_positions[1][10] == 0){block_positions[0][10] = x; if(game_string[x + 1] == 'K'){block_positions[1][10] = 1;}else{block_positions[1][10] = 2;}}
			if (game_string[x] == 'L' && block_positions[1][11] == 0){block_positions[0][11] = x; if(game_string[x + 1] == 'L'){block_positions[1][11] = 1;}else{block_positions[1][11] = 2;}}
			if (game_string[x] == 'M' && block_positions[1][12] == 0){block_positions[0][12] = x; if(game_string[x + 1] == 'M'){block_positions[1][12] = 1;}else{block_positions[1][12] = 2;}}
			if (game_string[x] == 'N' && block_positions[1][13] == 0){block_positions[0][13] = x; if(game_string[x + 1] == 'N'){block_positions[1][13] = 1;}else{block_positions[1][13] = 2;}}
			if (game_string[x] == 'O' && block_positions[1][14] == 0){block_positions[0][14] = x; if(game_string[x + 1] == 'O'){block_positions[1][14] = 1;}else{block_positions[1][14] = 2;}}
			if (game_string[x] == 'P' && block_positions[1][15] == 0){block_positions[0][15] = x; if(game_string[x + 1] == 'P'){block_positions[1][15] = 1;}else{block_positions[1][15] = 2;}}
			if (game_string[x] == 'Q' && block_positions[1][16] == 0){block_positions[0][16] = x; block_positions[1][16] = 1;}
			if (game_string[x] == 'Q' && block_positions[1][16] != 0){block_positions[0][17] = x; block_positions[1][17] = 1;}
		//Format of block_positions:
		//[[Position],[Rotation]]
		//Rotation 1 = horizontal, Rotation 2 = vertical
		}
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
	gfx_PrintInt((int)block_positions[0][18], 1);
}


void DrawBlocks(){//Draw all Blocks from block_positions
	uint8_t x, height, width;
	for(x = 0; x<16; x++){
		if(block_positions[1][x] == 0){continue;}//No block
		if(block_positions[1][x] == 2){height = 27; width = 59;}//Short block vertical
		if(block_positions[1][x] == 2 && x < 5 && x > 0){height = 27; width = 91;}//Long block vertical
		if(block_positions[1][x] == 1){height = 59; width = 27;}//Short block horizontal
		if(block_positions[1][x] == 1 && x < 5 && x > 0){height = 91; width = 27;}//Long block horizontal
		gfx_SetColor(252 - x);
		gfx_FillRectangle(67 + 32 * (block_positions[0][x] % 6), 32 * (block_positions[0][x] / 6) + 35, height, width);//Draw red block
	}
	if(block_positions[1][16] == 1){
		gfx_SetColor(236);
		gfx_FillRectangle(67 + 32 * (block_positions[0][16] % 6), 32 * (block_positions[0][16] / 6) + 35, 27, 27);//Draw first wall
	}
	if(block_positions[1][17] == 1){
		gfx_SetColor(236);
		gfx_FillRectangle(67 + 32 * (block_positions[0][17] % 6), 32 * (block_positions[0][17] / 6) + 35, 27, 27);//Draw second wall
	}
}

void DrawWinScreen(){//Draw the win win screen
	gfx_FillScreen(253);
	gfx_PrintStringXY("Congratulations! You have completed the level!", 2, 100);
	gfx_PrintStringXY("Press 2nd to return to the main menu...", 37, 120);
}


int CheckPos(uint8_t pos){//Check, which block occupies a position
	uint8_t x, block;
	block = 99;
	for(x = 0; x < 16; x++){
		if(block_positions[0][x] == pos && block_positions[1][x] != 0){block = x; break;}
	}
	for(x = 0; x < 16; x++){
		if(block == 99 && block_positions[1][x] == 1 && block_positions[0][x] == pos - 1 && block_positions[1][x] != 0){block = x; break;}
	}
	for(x = 0; x < 16; x++){
		if(block == 99 && block_positions[1][x] == 2 && block_positions[0][x] == pos - 6 && block_positions[1][x] != 0){block = x; break;}
	}
	if(block == 99 && block_positions[0][16] == pos && block_positions[1][x] != 0){block = 16;}
	if(block == 99 && block_positions[0][17] == pos && block_positions[1][x] != 0){block = 17;}
	for(x = 1; x < 5; x++){
		if(block == 99 && block_positions[1][x] == 1 && block_positions[0][x] == pos - 2 && block_positions[1][x] != 0){block = x; break;}
	}
	for(x = 1; x < 5; x++){
		if(block == 99 && block_positions[1][x] == 2 && block_positions[0][x] == pos - 12 && block_positions[1][x] != 0){block = x; break;}
	}
	return block;
}


void DrawCursor(uint8_t pos, uint8_t color){//Draw the cursor at given position
	uint8_t x, block;

	block = CheckPos(pos);//Check, which block is at position
	if(block == 99){//Draw the cursor just at pos if no block is at position
		gfx_SetColor(252 - color);
		gfx_FillRectangle(pos % 6 * 32 + 64, pos / 6 * 32 + 32, 32, 32);
	}
	else{
		for(x = 0; x < 36; x++){//Draw cursor at any position where a part of the block, which at pos is, is
			if(CheckPos(x) == block){
				gfx_SetColor(252 - color);
				gfx_FillRectangle(x % 6 * 32 + 64, x / 6 * 32 + 32, 32, 32);
			}
		}
	}
}



int main(void){

	uint16_t game_id, keycountC, keycountA, cursorpos, selectedblock, moves, latestblock, game_status;
	kb_key_t keyA, keyC, prevkeyA, prevkeyC;
	uint8_t highscores[1813] = {0};
	uint16_t* highscore_sv = (uint16_t*)&highscores;

	srandom(rtc_Time());//Set random seed
	game_id = 0;
	cursorpos = 0;
	moves = 0;
	selectedblock = 98;
	latestblock = 99;
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

	gfx_Begin(); //Start the graphics
  gfx_SetPalette(palette_gfx, sizeof_palette_gfx, 0); //Load Palette
	gfx_SetDrawBuffer();

	gfx_FillScreen(253);//Draw Background
	DrawTitle();
	gfx_SwapDraw();

	gfx_FillScreen(253);//Draw Background
	DrawTitle();

	while (!(kb_Data[6] == kb_Clear)){//Main loop

		//Controls
		if(kb_Data[7] == kb_Left && keyC != prevkeyC){//Left
			if(game_status == 0 && game_id > 0){game_id = game_id - 1;}
			if(game_status == 1 && cursorpos % 6 > 0 && selectedblock == 98){//Move cursor left
				if(CheckPos(cursorpos - 1) == 99){cursorpos = cursorpos - 1;}
				else if(CheckPos(cursorpos) != CheckPos(cursorpos - 1)){cursorpos = block_positions[0][CheckPos(cursorpos - 1)];}
				else if(CheckPos(cursorpos - 2) == 99){cursorpos = cursorpos - 2;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos - 1 ) && CheckPos(cursorpos) != CheckPos(cursorpos - 2) && cursorpos % 6 > 1){cursorpos = block_positions[0][CheckPos(cursorpos - 2)];}
				else if(CheckPos(cursorpos - 3) == 99){cursorpos = cursorpos - 3;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos - 1 ) && CheckPos(cursorpos) == CheckPos(cursorpos - 2) && cursorpos % 6 > 2){cursorpos = block_positions[0][CheckPos(cursorpos - 3)];}
			}
			if(game_status == 1 && cursorpos % 6 > 0 && selectedblock != 98 && CheckPos(block_positions[0][CheckPos(cursorpos)] - 1) == 99 && block_positions[1][CheckPos(cursorpos)] == 1){//Move block left
				block_positions[0][CheckPos(cursorpos)] = block_positions[0][CheckPos(cursorpos)] - 1;
				cursorpos = cursorpos - 1;
				if(CheckPos(cursorpos) != latestblock){
					moves = moves + 1;
				}
				latestblock = CheckPos(cursorpos);
			}
		}

		if(kb_Data[7] == kb_Right && keyC != prevkeyC){//Right
			if(game_status == 0 && game_id < 1810){game_id = game_id + 1;}
			if(game_status == 1 && cursorpos % 6 < 5 && selectedblock == 98){//Move cursor right
				if(CheckPos(cursorpos + 1) == 99){cursorpos = cursorpos + 1;}
				else if(CheckPos(cursorpos) != CheckPos(cursorpos + 1)){cursorpos = block_positions[0][CheckPos(cursorpos + 1)];}
				else if(CheckPos(cursorpos + 2) == 99){cursorpos = cursorpos + 2;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos + 1 ) && CheckPos(cursorpos) != CheckPos(cursorpos + 2) && cursorpos % 6 < 4){cursorpos = block_positions[0][CheckPos(cursorpos + 2)];}
				else if(CheckPos(cursorpos + 3) == 99){cursorpos = cursorpos + 3;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos + 1 ) && CheckPos(cursorpos) == CheckPos(cursorpos + 2) && cursorpos % 6 < 3){cursorpos = block_positions[0][CheckPos(cursorpos + 3)];}
			}
			if(game_status == 1 && selectedblock != 98 && (((CheckPos(cursorpos) == 0 || (CheckPos(cursorpos) > 4  && CheckPos(cursorpos) < 16)) && CheckPos(block_positions[0][CheckPos(cursorpos)] + 2) == 99 && cursorpos % 6 < 4)|| ((CheckPos(cursorpos) > 0  && CheckPos(cursorpos) < 5) && CheckPos(block_positions[0][CheckPos(cursorpos)] + 3) == 99 && cursorpos % 6 < 3)) && block_positions[1][CheckPos(cursorpos)] == 1){//Move block right
				block_positions[0][CheckPos(cursorpos)] = block_positions[0][CheckPos(cursorpos)] + 1;
				cursorpos = cursorpos + 1;
				if(CheckPos(cursorpos) != latestblock){
					moves = moves + 1;
				}
				latestblock = CheckPos(cursorpos);
				if(block_positions[0][0] == 16){//Win
					highscores[game_id] = moves;
					game_status = 2;
					cursorpos = 0;
					selectedblock = 98;
					moves = 0;
					latestblock = 99;
				}
			}
		}

		if(kb_Data[7] == kb_Up && keyC != prevkeyC){//Up
			if(game_status == 1 && cursorpos / 6 > 0 && selectedblock == 98){//Move cursor up
				if(CheckPos(cursorpos - 6) == 99){cursorpos = cursorpos - 6;}
				else if(CheckPos(cursorpos) != CheckPos(cursorpos - 6)){cursorpos = block_positions[0][CheckPos(cursorpos - 6)];}
				else if(CheckPos(cursorpos - 12) == 99){cursorpos = cursorpos - 12;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos - 6 ) && CheckPos(cursorpos) != CheckPos(cursorpos - 12) && cursorpos / 6 > 1){cursorpos = block_positions[0][CheckPos(cursorpos - 12)];}
				else if(CheckPos(cursorpos - 18) == 99){cursorpos = cursorpos - 18;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos - 6 ) && CheckPos(cursorpos) == CheckPos(cursorpos - 12) && cursorpos / 6 > 2){cursorpos = block_positions[0][CheckPos(cursorpos - 18)];}
			}
			if(game_status == 1 && cursorpos / 6 > 0 && selectedblock != 98 && CheckPos(block_positions[0][CheckPos(cursorpos)] - 6) == 99 && block_positions[1][CheckPos(cursorpos)] == 2){//Move block up
				block_positions[0][CheckPos(cursorpos)] = block_positions[0][CheckPos(cursorpos)] - 6;
				cursorpos = cursorpos - 6;
				if(CheckPos(cursorpos) != latestblock){
					moves = moves + 1;
				}
				latestblock = CheckPos(cursorpos);
			}
		}

		if(kb_Data[7] == kb_Down && keyC != prevkeyC){//Down
			if(game_status == 1 && cursorpos / 6 < 5 && selectedblock == 98){//Move cursor down
				if(CheckPos(cursorpos + 6) == 99){cursorpos = cursorpos + 6;}
				else if(CheckPos(cursorpos) != CheckPos(cursorpos + 6)){cursorpos = block_positions[0][CheckPos(cursorpos + 6)];}
				else if(CheckPos(cursorpos + 12) == 99){cursorpos = cursorpos + 12;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos + 6 ) && CheckPos(cursorpos) != CheckPos(cursorpos + 12) && cursorpos / 6 < 4){cursorpos = block_positions[0][CheckPos(cursorpos + 12)];}
				else if(CheckPos(cursorpos + 18) == 99){cursorpos = cursorpos + 18;}
				else if(CheckPos(cursorpos) == CheckPos(cursorpos + 6 ) && CheckPos(cursorpos) == CheckPos(cursorpos + 12) && cursorpos / 6 < 3){cursorpos = block_positions[0][CheckPos(cursorpos + 18)];}
			}

			if(game_status == 1 && selectedblock != 98 && ((((CheckPos(cursorpos) > 4  && CheckPos(cursorpos) < 16)) && CheckPos(block_positions[0][CheckPos(cursorpos)] + 12) == 99  && cursorpos / 6 < 4) || ((CheckPos(cursorpos) > 0  && CheckPos(cursorpos) < 5) && CheckPos(block_positions[0][CheckPos(cursorpos)] + 18) == 99 && cursorpos / 6 < 3)) && block_positions[1][CheckPos(cursorpos)] == 2){//Move block down
				block_positions[0][CheckPos(cursorpos)] = block_positions[0][CheckPos(cursorpos)] + 6;
				cursorpos = cursorpos + 6;
				if(CheckPos(cursorpos) != latestblock){
					moves = moves + 1;
				}
				latestblock = CheckPos(cursorpos);
			}
		}

		if(kb_Data[1] == kb_2nd && keyA != prevkeyA){//2nd
		 if (game_status == 0){game_status = 1;}
		 else if (game_status == 1 && CheckPos(cursorpos) != 99 && CheckPos(cursorpos) != 17 && CheckPos(cursorpos) != 16 && selectedblock == 98){//select block to move
			 selectedblock = CheckPos(cursorpos);
		 }
		 else if (game_status == 1 && selectedblock != 98){//unselect block
			 selectedblock = 98;
			 latestblock = 99;
		 }
		 if(game_status == 2){//Exit the win screen
			 game_status = 0;
			 cursorpos = 0;
			 selectedblock = 98;
			 moves = 0;
			 latestblock = 99;
			 gfx_FillScreen(253);
			 DrawTitle();
			 gfx_SwapDraw();
			 gfx_FillScreen(253);
			 DrawTitle();
		 }
		}

		if(kb_Data[1] == kb_Del && keyA != prevkeyA){//del
			if(game_status == 1){//Exit the level
				game_status = 0;
				cursorpos = 0;
				selectedblock = 98;
				moves = 0;
				latestblock = 99;
			}
			if(game_status == 2){//Exit the win screen
				game_status = 0;
				cursorpos = 0;
				selectedblock = 98;
				moves = 0;
				latestblock = 99;
				gfx_FillScreen(253);
 			  DrawTitle();
 			  gfx_SwapDraw();
 			  gfx_FillScreen(253);
 			  DrawTitle();
			}
			else{//Exit the game
				break;
			}
		}

		//Bottom menu navigation
		if(kb_Data[1] == kb_Yequ && game_id > 99 && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = game_id - 100;}
		if(kb_Data[1] == kb_Yequ && game_id < 100 && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = 0;}
		if(kb_Data[1] == kb_Window && game_id > 9 && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = game_id - 10;}
		if(kb_Data[1] == kb_Window && game_id < 10 && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = 0;}
		if(kb_Data[1] == kb_Zoom && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = random() % 1811;}
		if(kb_Data[1] == kb_Trace && game_id < 1801 && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = game_id + 10;}
		if(kb_Data[1] == kb_Trace && game_id > 1800 && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = 1810;}
		if(kb_Data[1] == kb_Graph && game_id < 1711 && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = game_id + 100;}
		if(kb_Data[1] == kb_Graph && game_id > 1710 && (keyA != prevkeyA || keycountA > 2) && game_status == 0){game_id = 1810;}



		if(game_status == 0){GetBlockPositionsFromId(game_id);}


		//Graphics
		gfx_SetColor(253);
		gfx_FillRectangle(64,32,193,193);//Redraw Background
		if(game_status == 1 && selectedblock == 98){DrawCursor(cursorpos, 0);}
		if(game_status == 1 && selectedblock != 98){DrawCursor(cursorpos, 3);}
		DrawBoard();
		DrawLevelInfo(game_id, moves, highscores[game_id]);
		gfx_SetColor(253);
		gfx_FillRectangle(10,230,300,8);
		DrawBlocks();
		if(game_status == 0){DrawMenu();}
		if(game_status == 2){DrawWinScreen();}
		gfx_SwapDraw();

		gfx_SetColor(253);
		gfx_FillRectangle(64,32,193,193);//Redraw Background
		if(game_status == 1 && selectedblock == 98){DrawCursor(cursorpos, 0);}
		if(game_status == 1 && selectedblock != 98){DrawCursor(cursorpos, 3);}
		DrawBoard();
		DrawLevelInfo(game_id, moves, highscores[game_id]);
		gfx_SetColor(253);
		gfx_FillRectangle(10,230,300,8);
		DrawBlocks();
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
  gfx_End();// Close the graphics
}
