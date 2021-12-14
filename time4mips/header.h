/* header.h
Header file for all labs.
This file written 2015 by F Lundevall
Some parts are original code written by Axel Isaksson

Latest update 2015-08-28 by F Lundevall

For copyright and licensing, see file COPYING */

/* ------------------ MIPSFUNCTIONS -----------------*/
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);
char * itoaconv( int num );
void game_state(void);
int nextprime( int inval );
void quicksleep(int cyc);
void tick( unsigned int * timep );
void display_debug( volatile int * const addr );
/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare bitmap array containing icon */
extern uint8_t gF[512];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

/* -------------------- LABWORK --------------------*/
void delay(int);
void time2string( char *, int );
void enable_interrupt(void);

/* -------------------- BUTTON --------------------*/
int get_buttons(void);
int get_switches(void);

/* ------------------- DISPLAY --------------------*/

void pixel(int x1, int y1, int x2, int y2);
void convert_pixel();
void clear_game_field();
uint8_t pixel_array[32][128];
uint8_t game_field[512];

/* -------------------- MENU ---------------------*/
void menu_cursor(int menu_Selector);
void clear_string_display();
void start_screen();
void end_cursor(int scores, char* initials);

/* ------------------- SCORE ---------------------*/
char* display_score(int number);

