#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */

void menuStartScreen(){
            display_string(0, "Flappy Bird 4711");
            display_string(1, " - Start   ");
            display_string(2, "   High Score");
}

void menu_cursor(int menu_Selector){
    display_string(0, "Flappy Bird 4711");
    switch (menu_Selector) {
        case 0:
            display_string(1, " - Start   ");
            display_string(2, "   High Score");
            break;
        case 1:
            display_string(1, "   Start   ");
            display_string(2, " - High Score");
            break;
    }
    display_update();
}

void endTitle_cursor(int endTitle_Selector, int scores){
    display_string(0, "Score");
    display_string(1, display_score(scores));
    switch (endTitle_Selector) {
        case 0:
            display_string(2, " - Retry");
            display_string(3, "   Return");
            break;
        case 1:
            display_string(2, "   Retry");
            display_string(3, " - Return");
            break;
    }
    display_update();
}

void clearStringDisplay(){
    display_string(0,"");
    display_string(1,"");
    display_string(2,"");
}

