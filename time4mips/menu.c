#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for these labs */

void start_screen(){
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

void end_cursor(int scores, char* initials){
    display_string(0, "Score");
    display_string(1, display_score(scores));
    display_string(2, initials);
    display_string(3, "Enter");
    display_update();
}

void clear_string_display(){
    display_string(0,"");
    display_string(1,"");
    display_string(2,"");
    display_string(3,"");
}

