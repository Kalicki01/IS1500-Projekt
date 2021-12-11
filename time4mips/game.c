/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* -------------------------------- BIRD ----------------------*/

struct BIRD{
    int x0, x1;
    int y0, y1;
    float velocity;
    float acceleration;
    int canFlap;
};

struct BIRD BIRD = {20, 21, 16, 17, 9, 2, 1};
/* -------------------------------- OBSTACLES ----------------------*/

struct OBSTACLE{
    int x0, x1;
    int y0;
    int gap;
    int y1;
};

// ------------------------- SET FPS
int FPS = 60;

    // Pipe 1
struct OBSTACLE p1 = { 62, 64, 5, 10, 20};
    // Pipe 2
struct OBSTACLE p2 = { 126, 128, 15, 10, 30};
// ---------- set start values for bird and pillars
// void setupGame(){
//     // Pipe 1
//     struct OBSTACLE p1 = { 62, 64, 5, 10, 20};
//     // Pipe 2
//     struct OBSTACLE p2 = { 126, 128, 15, 10, 30};
// }

int jumpSpeed = 9;
/* -------------------------------- SCORE --------------------------*/

int scores = 0;
int scoreCounter = 0;
char scoreToString[16] = "       ";

/* -------------------------------- COLLISION ----------------------*/

int collide = 1;

/* -------------------------------- Movement-------------------------*/

int gravity = 1;
int difficulty = 1;

/* -------------------------------- MENU ---------------------------*/

int in_menu = 1;
int menu_Selector = 0;
int menu_start_positioning = 0;

/* -------------------------------- HIGHSCORE ----------------------*/

int in_highscore = 0;

/* -------------------------------- End Title ----------------------*/

int end_title = 0; 
int endTitle_Selector = 0; 

/* -------------------------------- IN GAME ------------------------*/

int in_game = 0;
int game_start_positioning = 1;

volatile int *porte = (volatile int*) 0xbf886110; // Pointer for PORTE

int randomValue(int lim)
{
    static long a = 100001;
    a = (a * 125) % 2796203;
    return ((a % lim) + 1);
}

void score(struct OBSTACLE *pillar) {
    if (BIRD.x1 == pillar->x1) {
        scoreCounter++;
        (*porte)++;
    }
}

char* display_score(int number){
    return itoaconv(number);
}

void endTitle(){
        clearGameField();
        display_string(0, "Score");
        display_string(1,     display_score(scoreCounter));
        display_string(2, " - Retry");
        display_string(3, "   Return");
        display_update();
}

void collision(){
    // Collision with pillar 1
    if (((BIRD.x0 <= p1.x0) && (BIRD.x1 >= p1.x0)) && ((BIRD.y0 < p1.y0) || (BIRD.y1 > p1.y1 ))) {
        end_title = 1; in_game = 0; 
    }
    // Collision with pillar 2
    if (((BIRD.x0 <= p2.x0) && (BIRD.x1 >= p2.x0)) && ((BIRD.y0 < p2.y0) || (BIRD.y1 > p2.y1 ))) {
        end_title = 1; in_game = 0; 
    }
    endTitle();
}

/* -------------------------------- OUTLINE ----------------------*/

void drawOutline(){
    pixel(0,0,128,0); // Top line
    pixel(0,31,128,31);
}

/* -------------------------------- PILLAR ----------------------*/

void changeGap(struct OBSTACLE *pillar){
    if (scoreCounter == 10) pillar->gap = 8;
    if (scoreCounter == 20) pillar->gap = 7;
    if (scoreCounter == 30) pillar->gap = 6;
    if (scoreCounter == 40) pillar->gap = 5;
}

void drawPillar(struct OBSTACLE pillar){
    int yMin = 0;
    int yMax = 31;
    // Upper pillar
    pixel(pillar.x0, yMin, pillar.x1, pillar.y0);
    // Lower pillar
    pixel(pillar.x0, pillar.y1, pillar.x1, yMax);
}

void randomPos(struct OBSTACLE *pillar){
    int r = randomValue(31-(pillar->gap));
    pillar->y0 = r;
    pillar->y1 = r + pillar->gap;
}

/* -------------------------------- MOVEMENT ----------------------*/

void movePillar(struct OBSTACLE *pillar){
    if (pillar->x1 < 0){
        pillar->x0 = 130;
        pillar->x1 = pillar->x0+2;
        randomPos(pillar);
    }
        pillar->x0 -= difficulty;
        pillar->x1 -= difficulty;
    //else pillar->x0 = pillar->x0;
}

void move() {
    // y movement
    if ((getbtns() & 0x8 && BIRD.canFlap == 1)) {
        BIRD.velocity = jumpSpeed;
        BIRD.canFlap = 1;
    } else{
        BIRD.velocity += BIRD.acceleration * 1/FPS;
    }

    // x movement
    /*
    if(BIRD.x0 == 20 && getbtns() & 0x1){
            BIRD.x0 += (BIRD.velocity)+5;
            BIRD.x1 += (BIRD.velocity)+5;
    }
    if(BIRD.x0 > 20){
            BIRD.x0 -= gravity; 
            BIRD.x1 -= gravity;
    }
    */
    BIRD.y0 += BIRD.velocity * 1/FPS;
    BIRD.y1 += BIRD.velocity * 1/FPS;
}

void move2() {
    if ((BIRD.y0 > 1 && BIRD.y1 < 30)) {
        if ((getbtns() & 0x8 && BIRD.canFlap == 1)) {
            BIRD.y0 -= (BIRD.velocity);
            BIRD.y1 -= (BIRD.velocity);
        }
        if(BIRD.x0 == 20 && getbtns() & 0x1){
            BIRD.x0 += (BIRD.velocity)+5;
            BIRD.x1 += (BIRD.velocity)+5;
        }
        if(BIRD.x0 > 20){
            BIRD.x0 -= gravity; //
            BIRD.x1 -= gravity;
        }
        if ((getbtns() & 0x2)) {
            BIRD.y0 += (BIRD.velocity);
            BIRD.y1 += (BIRD.velocity);
        }
    }
}


/* -------------------------------- MENU SELECTION ----------------------*/

void menu_selector(void){
    delay(10);
    if(getbtns() & 0x8 && menu_Selector == 0)
    {
        in_menu = 0;
        game_start_positioning = 1;
        in_game = 1;
        end_title = 0; 
    }
    if(getbtns() & 0x2 && menu_Selector != 1)
    {
        menu_Selector++;
        menu_cursor(menu_Selector);
    }
    if(getbtns() & 0x4 && menu_Selector != 0)
    {
        menu_Selector--;
        menu_cursor(menu_Selector);
    }
}

void endTitle_selector(void){
    delay(10);
    if(getbtns() & 0x8 && endTitle_Selector == 0)
    {
        in_menu = 0;
        game_start_positioning = 1;
        in_game = 1;
        end_title = 0; 
    }
    if(getbtns() & 0x8 && endTitle_Selector == 1)
    {
        in_menu = 1;
        menu_start_positioning = 1;
        in_game = 0;
        end_title = 0; 
    }
    if(getbtns() & 0x2 && endTitle_Selector != 1)
    {
        endTitle_Selector++;
        endTitle_cursor(endTitle_Selector, scoreCounter);
    }
    if(getbtns() & 0x4 && endTitle_Selector != 0)
    {
        endTitle_Selector--;
        endTitle_cursor(endTitle_Selector, scoreCounter);
    }
}

/* -------------------------------- EXECUTION ----------------------*/

/* Interrupt Service Routine */
void user_isr( void )
{
    return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
    // timer config for fps counter
    volatile int *trise = (volatile int*) 0xbf886100;
    int masking = 0xff;
    masking = ~masking;
    *trise  = *trise  & masking;
    TRISDSET = 0x0fe0;
    T2CON = 0x70;
	TMR2 = 0;
	PR2 = (80000000 / 256) / 60; // 60 FPS 
	T2CONSET = (1 << 15);
    clearGameField();
    //setupGame();
}

void reset()
{
    p1.x0 = 62; p1.x1 = 64; p1.y0 = 5; p1.gap = 10; p1.y1 = 20;
    p2.x0 = 126;p2.x1 = 128; p2.y0 = 15; p2.gap = 10; p2.y1 = 30;
    BIRD.x0 = 20; BIRD.y0 = 21; BIRD.x1 = 16; BIRD.y1 = 17;
    clearGameField();
    (*porte) = 0;
    scoreCounter = 0;
}

void gameloop(){
        clearStringDisplay();
        pixel(BIRD.x0,BIRD.y0,BIRD.x1,BIRD.y1);
        drawPillar(p1); // Pillar One
        drawPillar(p2); // Pillar Two
        drawOutline();
        convert_pixel();
        display_image(0, gameField);
        move2();
        movePillar(&p1);
        movePillar(&p2);
        score(&p1);
        score(&p2);
        collision();
        //changeGap(&p1);
        //changeGap(&p2);
        clearGameField();
}

/* This function is called repetitively from the main program */
void labwork( void )
{
    if(in_game){
        /*
        Check if timer flag for gameloop is raised (delay is finished) and run gameloop
        */
        if(IFS(0) & 0x100){
            gameloop();
		    IFSCLR(0) = 0x100; // reset timer flag
        }
        /*
        Check timer for jump cooldown
        if(){
            BIRD.canFlap = 1;
		    IFSCLR(0) = 0x100; // reset timer flag
        }
        */
    }
    else if (in_menu) {
      menu_selector();
      if(menu_start_positioning){
            clearStringDisplay();
            menuStartScreen();
            menu_start_positioning = 0;
     }
    }
    else if (end_title){
            endTitle_selector();
    }
}