// Name: Ryan Murphy
// Game: 2FAT2FURIOUS2

#include "myLib.h"

#include "start.h"
#include "shy_left.h"
#include "shy_right.h"
#include "shy_right_green.h"
#include "shy_right_red.h"
#include "level1.h"
#include "button1.h"
#include "button2.h"
#include "button3.h"
#include "fridge.h"
#include "time.h"
#include "lv1start.h"
#include "level2.h"
#include "lv2start.h"
#include "end.h"

// THE HIGHLY ANTICIPATED SEQUAL TO BESTSELLER "2FAT2FURIOUS"
// I PRESENT TO YOU
// 2FAT2FURIOUS2

int state = 0; // current screen
int timer = 0;
int clock = 0;

int row = 5; // starting position
int col = 0;
int speed = 1;

int green = 0; // shadow variables
int greenRow = 0;
int greenCol = 0; 
int red = 0;
int redRow = 0;
int redCol = 0;
const unsigned short *greenDirection = shy_right_green;
const unsigned short *redDirection = shy_right_red;
const unsigned short *direction = shy_right;

int a = 0; // button variables
int aActive = 0;
int b = 0;
int bActive = 0;
int up = 0;
int down = 0;
int left = 0;
int right = 0;

int key = 0;

struct level // format of the levels
{
	int level;
	int endTime;
	int oneX;
	int oneY;
	int twoX;
	int twoY;
	int threeX;
	int threeY;
	int fridgeX;
	int fridgeY;
	const unsigned short *lv;
	const unsigned short *lvStart;
	int map[8][13];
}; // this format makes it really easy to add more levels

struct level one = {1, 10, 20, 120, 40, 60, 160, 100, 120, 0, level1, lv1start,
{
	{5,5,5,5,5,5,4,5,5,5,5,5},
	{5,0,0,0,0,0,0,0,0,0,0,5},
	{5,0,0,5,0,0,0,0,0,5,0,5},
	{5,0,2,5,0,0,0,0,5,5,0,5},
	{5,5,5,5,5,5,0,5,5,0,0,5},
	{0,0,0,0,0,0,0,5,3,0,0,5},
	{5,1,0,0,0,0,0,0,0,0,0,5},
	{5,5,5,5,5,5,5,5,5,5,5,5}
}
};

struct level two = {2, 18, 40, 80, 200, 120, 200, 60, 120, 0, level2, lv2start,
{
	{5,5,5,5,5,5,4,5,5,5,5,5},
	{5,0,0,0,0,0,0,0,0,0,0,5},
	{5,0,0,0,0,5,5,0,0,5,0,5},
	{5,0,0,0,5,5,0,0,0,0,3,5},
	{5,0,1,5,0,0,0,5,5,5,5,5},
	{0,0,5,5,0,5,0,5,0,0,0,5},
	{5,0,0,0,0,5,0,0,0,5,2,5},
	{5,5,5,5,5,5,5,5,5,5,5,5}
}
};

struct level current;

/*
 *  12 hours and 500 lines of code later...
 *  it is finally done
 */
int main() {

	REG_DISPCTL = MODE3 | BG2_ENABLE;

	current = one;

	while(1) {

		int oldRow = row;
		int oldCol = col;

		if (clock == current.endTime) state = 3; //"too slow" ending

		if (state == 0) { // title screen
			drawImage3(0, 0, START_WIDTH, START_HEIGHT, start);
			while (!KEY_DOWN_NOW(BUTTON_START));
			drawImage3(0, 0, LV1START_WIDTH, LV1START_HEIGHT, lv1start);
			while (KEY_DOWN_NOW(BUTTON_START));
			while (!KEY_DOWN_NOW(BUTTON_START));

			current = one;
			reset();
		}

		if (state == 4) { // end screen
			drawImage3(0, 0, END_WIDTH, END_HEIGHT, end);
			while (!KEY_DOWN_NOW(BUTTON_START));
			drawImage3(0, 0, LV1START_WIDTH, LV1START_HEIGHT, lv1start);
			while (KEY_DOWN_NOW(BUTTON_START));
			while (!KEY_DOWN_NOW(BUTTON_START));

			current = one;
			reset();
		}

		if (state == 1) { //the gameplay

			if(KEY_DOWN_NOW(BUTTON_RIGHT) && !right && current.map[row][col+1]!=5) {
				direction = shy_right;
				col+= speed;
				right = 1;
			}
			if(KEY_DOWN_NOW(BUTTON_LEFT) && !left && current.map[row][col-1]!=5 && col != 0) {
				direction = shy_left;
				col-= speed;
				left = 1;
			}
			if(KEY_DOWN_NOW(BUTTON_UP) && !up && current.map[row-1][col]!=5 && row != 0) {
				row-= speed;
				up = 1;
			}
			if(KEY_DOWN_NOW(BUTTON_DOWN) && !down && current.map[row+1][col]!=5) {
				row+= speed;
				down = 1;
			}
			if(KEY_DOWN_NOW(BUTTON_A)) {
				if (a == 0 && !aActive) { // create green shadow
					a = 1;
					green = 1;
					greenRow = row;
					greenCol = col;
				}
				if (aActive) { // return to green shadow
					a = 1;
					row = greenRow;
					col = greenCol;
					green = 0;
				}
				
			}
			if(KEY_DOWN_NOW(BUTTON_B)) {
				if (b == 0 && !bActive) { // create red portal
					b = 1;
					red = 1;
					redRow = row;
					redCol = col;
				}
				if (bActive) { // return to red shadow
					b = 1;
					row = redRow;
					col = redCol;
					red = 0;
				}
			}

			if (a) { // check if green portal is active (has A been let go)
				if(!KEY_DOWN_NOW(BUTTON_A)) { 
					a = 0;
					if (!aActive) {
						aActive = 1;
					} else {
						aActive = 0;
					}
				}
			}
			if (b) { // check if red portal is active (has b been let go)
				if(!KEY_DOWN_NOW(BUTTON_B)) {
					b = 0;
					if (!bActive) {
						bActive = 1;
					} else {
						bActive = 0;
					}
				}
			}
			pressButton();
			letGoOfKey();
			waitForVblank();
			
			if(green) { // draw green shadow if active
				drawImage3(greenRow * 20, greenCol * 20, SHY_RIGHT_WIDTH, SHY_RIGHT_HEIGHT, greenDirection);
			}
			if (red) { // draw red shadow if active
				drawImage3(redRow * 20, redCol * 20, SHY_RIGHT_WIDTH, SHY_RIGHT_HEIGHT, redDirection);
			}
			// draw buttons and fridge
			drawImage3(current.oneY, current.oneX, 20, 20, button1);
			drawImage3(current.twoY, current.twoX, 20, 20, button2);
			drawImage3(current.threeY, current.threeX, 20, 20, button3);
			drawImage3(current.fridgeY, current.fridgeX, 20, 20, fridge);
			//erase old location
			drawRect(oldRow * 20, oldCol * 20, SHY_RIGHT_WIDTH, SHY_RIGHT_HEIGHT, WHITE);
			drawImage3(row * 20, col * 20, SHY_RIGHT_WIDTH, SHY_RIGHT_HEIGHT, direction);

			timer++;
			if (timer >= 60) { // ~1 second
				clock++;
				timer = 0;
				drawRect(20, 54, 10, 18, WHITE);
			}

			drawString(20, 20, "Timer:", BLACK);
			int currentTime = 47 + current.endTime - clock;
			if (currentTime > 57) {
				drawChar(20, 54, 49, BLACK);
				drawChar(20, 59, currentTime - 10, BLACK);
			} else {
				drawChar(20, 59, currentTime, BLACK);
			}
			// Only supports times < 20 seconds right now
			// It would be an easy fix to support higher times
			// by using a while loop that -10 every time
			// but I've already spent too much time on this
		}

		if (state == 3) { // time up
			drawImage3(0, 0, TIME_WIDTH, TIME_HEIGHT, time);
			while (!KEY_DOWN_NOW(BUTTON_START));
			reset();
		}	
	}
}

/*
 *  This method starts the next level and resets all the important variables
 */
void reset() {
	timer = 0;
	clock = 0;
	state = 1;
	key = 0;

	a = 0;
	aActive = 0;
	green = 0;
	b = 0;
	bActive = 0;
	red = 0;

	row = 5;
	col = 0;

	drawImage3(0, 0, 240, 160, current.lv);
}
/*
 *	This method checks what tile the player is standing on
 *	it checks for buttons and the fridge
 * 	if you are on the fridge and hit the right buttons, then you go to the next level
 *  
 *  This method is really not optimized for multiple levels - I couldn't find a simple
 * 	way to automate this besides brute force
 */
void pressButton() {
	if (current.level == 1) {//2132
		if (current.map[row][col] == 1 && key == 2) key++;
		if (current.map[row][col] == 2 && (key == 0 || key == 6)) key+=2;
		if (current.map[row][col] == 3 && key == 3) key+=3;

		if (current.map[row][col] == 4 && key == 8) { // next level
			current = two;
			drawImage3(0, 0, LV1START_WIDTH, LV1START_HEIGHT, lv2start);
			while (KEY_DOWN_NOW(BUTTON_START));
			while (!KEY_DOWN_NOW(BUTTON_START));
			reset();
		}
	}
	if (current.level == 2) {//23212
		if (current.map[row][col] == 1 && key == 7) key++;
		if (current.map[row][col] == 2 && (key == 0 || key == 5 || key == 8)) key+=2;
		if (current.map[row][col] == 3 && key == 2) key+=3;

		if (current.map[row][col] == 4 && key == 10) { // you win
			state = 4;
		}
	}
}

/*
 *  This method makes sure you only move once per key stroke
 */
void letGoOfKey() {
	if (up) {
		if(!KEY_DOWN_NOW(BUTTON_UP)) up = 0;
	}
	if (down) {
		if(!KEY_DOWN_NOW(BUTTON_DOWN)) down = 0;
	}
	if (left) {
		if(!KEY_DOWN_NOW(BUTTON_LEFT)) left = 0;
	}
	if (right) {
		if(!KEY_DOWN_NOW(BUTTON_RIGHT)) right = 0;
	}
}

/*

                         ,,,,,,,,,,,,,
                   .,,,,,,,,,,,,,,,. .%%%&&&&&&&%%
                   .,,,,,,,,,,,,,,,. .%&&@@@@@@@&%
                 .....,,,,,,,,,,,  /&&@@@@@@@@@@@@&%&%
                .......,,........  /@@@@@@@@@@@@@@@@@&
                ............     ..(@@@@@&&&&&&&@@&&&%,...
                ...      &@@/    %&@@@@@@.     /@&   .%&&%
                   *%####%%%*    &@@@@@@@.     /@&   .&@@&
                   *########,    &@@@@@@@.     /@&   .&@@&.
                   ,#((((((#,    &@@@@@@@.     /@&   .&@@&
                                 %&@@@@@@@@@@@@@@@@@@@@@@@@@*
                 ................  /@&@@@@@@@@@@@@@@@@@@@@@@/
                 ................  /@&@@@@@@@@@@@@@@@@@@@@@@/
           ..,,,,,,,,,,,,,,,,....  /@&&&&@@@@@@@@@@@@@@@@@@@/   ...
       ,..,,,,,,,,,,,,,,,,,,,,,,,,,   %&&&&&&@@@@&   .&@@@&&*  .,,,...
       ,,.,,,,,,,,,,,,,,,,,,,,,,,,,. .#%%%%%%@@@@&,..,&@@@%%*  .,,,.,.
    ...,,,,,,,,,,..,,,,,,,,,,,,,,,,,..       %@&&&&&&&&&&%   ..,,,,,,,..
.,.,,,,,,,,,,..........,,,,,,,,,,,,,,,,,,,..,             ..,,,.   ,,,,,.
.,,,,,,,,,,,,..........,,,,,,,,,,,,,,,,,,,,,,.............,,,,,. .....,,
.,,,,,,..................,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,...   .,
 ..........      ........,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
 ..........      ........,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
           ..       ,.......,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,...
       ,.......,       ..........,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,...
       ......,,,.           .....,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
       ......,,,.            .........,,,,,,,,,,,,,,,,,,,,,,,,,....
    ............,,,,...                  ....,,,,,....                ..
    ...............,,,,,,...                          (###&&&&&/   .....
    ...............,,,,,,,,,.............             ####%%%%&*   ,,,..
    ,.................,,,,,,,,,,,,,,,,...             #%%%%%%%%*   ,,,,,
       .....................,,,,,,,,,,,,,,,,,,,,,,,...          ,.....
       ..............................,,,,,,,,,,,,,,,,,,,,,,,,.........
       ,..............................,,,,,,,,,,,,,,,,,,,,,,,.........
             .,..................................................,.
*#(&@@@(#####       ...........................................   .####(&@@@%#(
(@@%(##((((((((((((,     .................................     ,(((((((((##(%@%
(@@%((((((((((((((#,     .................................     ,#(((((((((((%@%
*##(((((((((((((((#,                                        *((((((((((((((((#(
*#((((((((((((((((#,                                        *#(((((((((((((((#/
.******************.                                        .*****************,

*/