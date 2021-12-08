/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint8_t pixel_array[32][128];

// Bird
int x1 = 20; int y1 = 16;
int x2 = 21; int y2 = 17;
int pixelSpeed = 5;

// Pipe 1
int p0_x_0 = 100;
int p0_y_0 = 5;
int p0_y_1 = 20;

// Pipe 2
int p1_x_0 = 300;
int p1_y_0 = 15;
int p1_y_1 = 30;

// Score
int scores = 0;
int scoreCounter = 0;

volatile int *porte = (volatile int*) 0xbf886110; // Pointer for PORTE

void pixel(int x1, int y1, int x2, int y2)
{
    int rows, columns;
    for (rows = 0; rows < 32; rows++) {
        for (columns = 0; columns < 128; columns++) {
            if ((rows >= y1 && rows <= y2) && (columns >= x1 && columns <= x2))
            {
                pixel_array[rows][columns] = 1;
            }
        }
    }
}
/*
void collision(){
    if (x2)
}
*/

// if ( (xPosBall >= xPosPadel1) && (xPosBall <= (xPosPadel1 + x2PosPadel1)) && ((yPosBall + ballSize) > yPosPadel1) && (yPosBall < (yPosPadel1 + y2PosPadel1)))

void score(int *pillarX, int *birdX){
    if (&birdX > &pillarX + 5) scores = 1;
    if (scores){
        scoreCounter++;
        (*porte)++;
        scores = 0;
    }
}


int randomValue(int lim)
{
    static long a = 100001;
    a = (a * 125) % 2796203;
    return ((a % lim) + 1);
}


void drawPillar(int x0, int y0, int y1){
    int yMin = 0;
    int yMax = 32;
    // Upper pillar
    pixel(x0, yMin, x0, y0);
    // Lower pillar
    pixel(x0, y1, x0, yMax);
}

void randomPos(int *pY0, int *pY1){
    int r = randomValue(17);
    *pY0 = r;
    *pY1 = r + 15;
}

void movePillar(int *pX0, int *pY0, int *pY1){
    if (*pX0 < 0){
        *pX0 = 130;
        randomPos(pY0, pY1);
    }
    *pX0 -= 1;
}


/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

void clearIcon(){
    int a,b;
    int i;
    for(a = 0; a < 128; a++){
        for (b = 0; b < 32; b++) {
            pixel_array[b][a] = 0;
        }
    }
    for(i = 0; i < 512; i++){
        icon[i] = 0;
    }
}



void convert_pixel(){
    int page, byte, bit;
    uint8_t sum = 0;
    uint8_t bitIncrementer = 1;
    for(page = 0; page < 4; page++){
        for(byte = 0; byte < 128; byte++){
            sum = 0; bitIncrementer = 1;  // We want to reset the binary-incrementer and sum each time we select the next byte (column).
            for(bit = 0; bit < 8; bit++){
                if(pixel_array[page*8+bit][byte] == 1){
                    sum += bitIncrementer;
                }
                bitIncrementer = bitIncrementer << 1; // We increment the binary-incrementer each time we select the next row (2^0, 2^1, 2^2, ....... , 2^7)
            }
            icon[page*128+byte] = sum;
        }
    }
}

/*
void pixels(int pageStart, int pageEnd, int start, int end, int bitStart, int bitEnd){
    int t, i, j;
    int sum = 0;
    int bitIncrementer = 1;
    for(t = pageStart; t < pageEnd; t++){
    for(i = start; i < end; i++){
        bitIncrementer = 1;
        sum = 0;
        for (j = 0; j < 8; j++){
            if(j >= bitStart && j <= bitEnd){
                sum |= bitIncrementer;
            }
            bitIncrementer <<= 1;
        }
        icon[t*128+i] = sum;
    }
  }
}
*/

void move(){
    if((getbtns() & 0x2) && x2 < 125){ x1 += pixelSpeed; x2 += pixelSpeed; }
    if((getbtns() & 0x4) && x1 >   0){ x1 -= pixelSpeed; x2 -= pixelSpeed; }
}

/* Lab-specific initialization goes here */
void labinit( void )
{
    volatile int *trise = (volatile int*) 0xbf886100;
    int masking = 0xff;
    masking = ~masking;
    *trise  = *trise  & masking;
    TRISDSET = 0x0fe0;
    clearIcon();
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  delay(10);
  pixel(x1,y1,x2,y2);
  drawPillar(p0_x_0,p0_y_0,p0_y_1); // Pillar One
  //drawPillar(p1_x_0,p1_y_0,p1_y_1); // Pillar Two
  convert_pixel();
  display_image(0, icon);
  move();
  movePillar(&p0_x_0, &p0_y_0, &p0_y_1);
  //movePillar(&p1_x_0, &p1_y_0, &p1_y_1);
  score(&p0_x_0,&x1);
  clearIcon();
}
