#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

/*Menu*/
int menu_Selector = 0;
int in_menu = 1;
int menu_start_positioning = 0;

/*Start*/
int in_game = 0;

/*Catagory*/
int in_category = 0;
int category_Selector = 0;
int category_start_positioning = 0;



int mytime = 0x5957;
volatile int *trise = (volatile int*) 0xbf886100; // Pointer for TRISE
volatile int *porte = (volatile int*) 0xbf886110; // Pointer for PORTE
int timeoutcount = 0;

/* Interrupt Service Routine */
void user_isr( void )
{

}

/* Lab-specific initialization goes here */
void labinit( void )
{
// Conclusion : We want to bit mask the address of TRISE and set the first 8 bits as output, meaning '0'
int masking = 0xff; // bits 7 through 0 (first 8 bits)
masking = ~masking; // invert
*trise  = *trise  & masking;
// Conclusion : Instruction tells us to set 5-11 bits of TRISD to 1 as 0x0fe0 = 0000 1111 1111 0000
TRISDSET = 0x0fe0; // we set button 2-4 and switch 1-4 to input (RD5 to RD11)
TRISFSET = 0x2;   //  we set button 1 to input (RF1)
}

void menu_cursor(void){
    display_string(0, "Ping Pong ");
    switch (menu_Selector) {
        case 0:
            display_string(1, " - Start   ");
            display_string(2, "   Game Modes");
            display_string(3, "   High Score");
            break;
        case 1:
            display_string(1, "   Start   ");
            display_string(2, " - Game Modes");
            display_string(3, "   High Score");
            break;
        case 2:
            display_string(1, "   Start   ");
            display_string(2, "   Game Modes");
            display_string(3, " - High Score");
            break;
    }
    display_update();
}
void category_cursor(void){
    display_string(0, "Category ");
    switch (category_Selector) {
        case 0:
            display_string(1, " - P vs P");
            display_string(2, "   P vs AI");
            break;
        case 1:
            display_string(1, "   P vs P");
            display_string(2, " - P vs AI");
            break;
    }
    display_update();
}

void clear_display(){
    display_string(0, "");
    display_string(1, "");
    display_string(2, "");
    display_string(3, "");
}

void menu_selector(void){
    quicksleep(80000000/ 200);
    if(getbtns() & 0x2 && menu_Selector == 1){in_menu = 0; in_category = 1; category_start_positioning = 1;}
    if(getbtns() & 0x4 && menu_Selector != 2){menu_Selector++; menu_cursor();}
    if(getbtns() & 0x8 && menu_Selector != 0){menu_Selector--; menu_cursor();}
}

void category_selector(void){
    quicksleep(80000000/ 200);
    if(getbtns() & 0x1){ in_category = 0; in_menu = 1;  menu_start_positioning = 1;}
    if(getbtns() & 0x4 && category_Selector != 1){category_Selector++; category_cursor();}
    if(getbtns() & 0x8 && category_Selector != 0){category_Selector--; category_cursor();}
}

/* This function is called repetitively from the main program */
void labwork( void )
{
    quicksleep(80000000/ 256);
    if (in_menu){
        menu_selector();
        if(menu_start_positioning){
            clear_display();
            display_string(0, "Ping Pong ");
            display_string(1, " - Start   ");
            display_string(2, "   Game Modes");
            display_string(3, "   High Score");
            menu_start_positioning = 0;
        }
    }
    if (in_category){
        category_selector();
        if(category_start_positioning){
            clear_display();
            display_string(0, "Category ");
            display_string(1, " - P vs P");
            display_string(2, "   P vs AI");
            category_start_positioning = 0;
        }
    }
    display_update();
}
