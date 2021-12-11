#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"   /* Declatations for these labs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t pixel_array[32][128];

void pixel(int x1, int y1, int x2, int y2)
{
    int rows,columns;
    for (rows = 0; rows < 32; rows++) {
        for (columns = 0; columns < 128; columns++) {
            if ((rows >= y1 && rows <= y2) && (columns >= x1 && columns <= x2))
            {
                pixel_array[rows][columns] = 1;
            }
        }
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
            gameField[page*128+byte] = sum;
        }
    }
}


void clearGameField(){
    int a,b;
    int i;
    for(a = 0; a < 128; a++){
        for (b = 0; b < 32; b++) {
            pixel_array[b][a] = 0;
        }
    }
    for(i = 0; i < 512; i++){
        gameField[i] = 0;
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






