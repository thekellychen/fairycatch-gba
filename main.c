#include <stdio.h>
#include <stdlib.h>
#include "myLib.h"
#include "text.h"
#include "start.h"
#include "winGame.h"
#include "gameOver.h"
#include "game.h"
#include "tink.h"
#include "flower.h"
#include "beast.h"

enum {OPEN, PLAY, WIN, LOSE};

//Prototypes
void open(); 
int play();
void win();
void lose();

//Scores
#define SCORE_INITIAL 5000
char scoreArray[41];
int score;


int main() {
	int curState = OPEN;
	while(1)
	{
		switch(curState) {
			case OPEN:
				open();
				curState = PLAY;
				break;
			case PLAY:
				curState = play();
				break;
			case WIN:
				win();
				curState = OPEN;
				break;
			case LOSE:
				lose();
				curState = OPEN;
				break;
			default:
				break;
		}
	}
}

void open()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	// Welcome Screen
	drawImage3(0, 0, START_WIDTH, START_HEIGHT, start);
	drawString(115, 30, "Press A to throw the flower!", YELLOW);
	drawString(125, 0, "Move TinkerBell using UP/DOWN/LEFT/RIGHT", YELLOW);
	drawString(135, 55, "Press START to begin!", YELLOW);

	while (!KEY_DOWN_NOW(BUTTON_START));
	while (KEY_DOWN_NOW(BUTTON_START));
}

int play()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	//Draw background
	drawRectangle(0, 0, 240, 160, WHITE);
	//Initiate score
	drawString(150, 5, scoreArray, MAGENTA);

	//Draw Tink
	Tink tink_var = {5, 5};
	Tink *tptr = &tink_var;
	Tink old_tink_var = tink_var;
	drawTink(tptr);

	//Draw Beast
	Beast beast_var={80,150};
	Beast *bptr = &beast_var;
	Beast old_beast_var = beast_var;
	drawBeast(bptr);
	
	//Draw flower
	Flower flower_var = {(tptr->posY) + TINK_HEIGHT, (tptr->posX) + TINK_WIDTH-5};
	Flower *fptr = &flower_var;
	Flower old_flower_var = flower_var;

	//Initiate other variables
	int tinkPrevPos = tptr->posY;
	score = SCORE_INITIAL;
	Boolean release = 0;
	int randomInt;
	int time = 0;
	int sx, sy;

	while(1) {
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			score = SCORE_INITIAL;
			return OPEN;
		}

		//Move Beast in circular motion
		sx = 33*COS(time);
		sy = 33*SIN(time);
		bptr->posX=(sx>>12) + 150;
		bptr->posY=(sy>>12) + 80;
		time++;
		if (time==0xFFFF) {
			time = 0;
		}

		//Move Tink up
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			if (tptr->posY>10) {
		    	tptr->posY--;
		    	tptr->posY--;
		  	}
		}

		//Move Tink down
		if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			if (tptr->posY<160-TINK_HEIGHT-10) {
		    	tptr->posY++;
		    	tptr->posY++;
		  	}
		}

		//Move Tink right
		if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
			if (tptr->posX<240-TINK_WIDTH-10) {
				tptr->posX++;
				tptr->posX++;
			}
		}

		//Move Tink left
		if (KEY_DOWN_NOW(BUTTON_LEFT)) {
			if (tptr->posX > 10) {
				tptr->posX--;
				tptr->posX--;
			}
		}

		//Release flower
		if (KEY_DOWN_NOW(BUTTON_A)) {
			release = 1;
			tinkPrevPos = tink_var.posY;
		}
		if (release) {
			flower_var.posY = tinkPrevPos;
			drawFlower(fptr);
			if (fptr->posX<240-FLOWER_WIDTH-10) {
				fptr->posX++;
			}
			//Erase old flower
			drawRectangle(old_flower_var.posY, old_flower_var.posX, FLOWER_WIDTH, FLOWER_HEIGHT, WHITE);
			//Draw new Flower
			drawFlower(fptr);
			old_flower_var = flower_var;
		}

		//Flower reaches other side without colliding
		if (fptr->posX == 240-FLOWER_WIDTH-10 && !collide(fptr, bptr)) {
			return LOSE;
		}

		//Flower collides with Beast
		if (collide(fptr, bptr)) {
			return WIN;
		}

		//Update the score and print it
		randomInt = rand()%10;
		if (randomInt == 5) {
			score--;
		}
		sprintf(scoreArray, "Score: %d", score);

		waitForVblank();
		//Erase the old Tink
		drawRectangle(old_tink_var.posY, old_tink_var.posX, TINK_WIDTH, TINK_HEIGHT, WHITE);
		//Draw new Tink
		drawTink(tptr);
		old_tink_var = tink_var;
		//Erase old Beast
		drawRectangle(old_beast_var.posY, old_beast_var.posX, BEAST_WIDTH, BEAST_HEIGHT, WHITE);
		//Draw new Beast
		drawBeast(bptr);
		old_beast_var = beast_var;
		//Erase old score and draw new score
		drawRectangle(150, 40, 35, 10, WHITE);
		drawString(150, 5, scoreArray, MAGENTA);
	}
}


void lose()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	drawImage3(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameOver);
	drawString(40, 60, "Oh no! The beast fled!", WHITE);
	drawString(55, 45, "Press START to try again!", WHITE);
	while (!KEY_DOWN_NOW(BUTTON_START));
	while (KEY_DOWN_NOW(BUTTON_START));
}

void win()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	drawImage3(0, 0, WINGAME_WIDTH, WINGAME_HEIGHT, winGame);
	drawString(60, 40, "Congrats! The beast was caught!", BLUE);
	drawString(70, 40, "Your score was:", BLUE);
	sprintf(scoreArray, "%d", score);
	drawString(70, 135, scoreArray, BLUE);
	drawString(90, 40, "Press START to play again!", BLUE);
	while (!KEY_DOWN_NOW(BUTTON_START));
	while (KEY_DOWN_NOW(BUTTON_START));
}

