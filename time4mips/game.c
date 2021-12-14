/* game.c

   This file was written 2021 by Anton Brömster and Anton Schulz.

*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header.h"  /* Declatations for all files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* -------------------------------- BIRD ----------------------*/

struct BIRD{
    int x0, x1;
    int y0, y1;
    float velocity;
    float acceleration;
};

struct BIRD BIRD = {20, 21, 16, 17, 0, 0};

/* -------------------------------- OBSTACLES ------------------*/

struct OBSTACLE{
    int x0, x1;
    int y0;
    int gap;
    int y1;
};

struct OBSTACLE p1 = { 128, 130, 5, 20, 25};    // Pipe 1
    
struct OBSTACLE p2 = { 256, 258, 10, 20, 30}; // Pipe 2

struct OBSTACLE p3 = { 384, 386, 10, 20, 30}; // Pipe 2

struct OBSTACLE p4 = { 512, 514, 5, 20, 25};    // Pipe 1

/* -------------------------------- SCORE & HIGHSCORE-------------*/

int score = 0;
char initials[4] = "AAA";
int highscore[3] = {0 , 0 , 0};
char leaderboard_stats_one[16] = "";
char leaderboard_stats_two[16] = "";
char leaderboard_stats_three[16] = "";

/* -------------------------------- MOVEMENT-------------------------*/

int pillar_distance = 128;
float gravity = 20.0f;
float xvelocity = 0;

/* -------------------------------- NAVIGATION -----------------------*/

int in_menu = 1;
int in_game = 0;
int in_end  = 0;
int in_highscore = 0;

/* -------------------------------- SELECTORS -----------------------*/

int end_Selector = 0;
int menu_Selector = 0;

/* -------------------------------- START-POS -----------------------*/

int menu_start_positioning = 0;
int in_game_start_positioning = 0;
int end_game_start_positioning = 0;
int highScore_start_positioning = 0;



volatile int *porte = (volatile int*) 0xbf886110; // Pointer for PORTE
int timeoutcount = 1;
int collide = 0;
int is_reset = 0;
int FPS = 60;

/* -------------------------------- FUNCTIONS -----------------------*/

/*
    change_highscore(char* name) 
    
    Will change the highscore-list depending on the given input value and the value of the players score. 
    The array "highscore" is updated and the char-arrays (leaderboard_stats_(one-three)). 
*/

void change_highscore(char* name){
    char temp[16] = "";
    // If score is greater than the #1 score
    if(score >= highscore[0]){
            int rank2 = highscore[0];
            int rank3 = highscore[1];
            highscore[0] = score;
            highscore[1] = rank2;
            highscore[2] = rank3;
            strcpy(leaderboard_stats_three, leaderboard_stats_two);
            strcpy(leaderboard_stats_two, leaderboard_stats_one);
            strcat(temp, name);
            strcat(temp, " - ");
            strcat(temp, display_score(highscore[0]));
            strcpy(leaderboard_stats_one, temp);
      // If score is smaller than #1 score but bigger than the #2 score
    } else if (score >= highscore[1] && score <= highscore[0]) {
            int rank3 = highscore[1];
            highscore[1] = score;
            highscore[2] = rank3;
            strcpy(leaderboard_stats_three, leaderboard_stats_two);
            strcat(temp, name);
            strcat(temp, " - ");
            strcat(temp, display_score(highscore[1]));
            strcpy(leaderboard_stats_two, temp);
        // If score is smaller than #1,#2 score but bigger than the #3 score
    }  else if (score >= highscore[2] && score <= highscore[1]) {
            highscore[2] = score;
            strcat(temp, name);
            strcat(temp, " - ");
            strcat(temp, display_score(highscore[2]));
            strcpy(leaderboard_stats_three, temp);
    }
}

void display_highscore(){
    display_string(0, "Highscore");
    display_string(1, leaderboard_stats_one);
    display_string(2, leaderboard_stats_two);
    display_string(3, leaderboard_stats_three);
    display_update();
}

int random_value(int lim)
{
    static long a = 100001;
    a = (a * 125) % 2796203;
    return ((a % lim) + 1);
}

/*
    change_score(struct OBSTACLE *pillar)

    Will increment the global variable score if the positing of the bird, has passed the positing of the pillar (x-coordinate).
    (This will also alter the leds.) 
*/

void change_score(struct OBSTACLE *pillar) {
    if (BIRD.x1 == pillar->x1) {
        score++;
        (*porte)++;
    }
}

/*
    display_score(int number)

    Will convert integers to charpointer. 
    This is done by calling on the variable-to-string function that is written in mipslabfunc (mipsFunctions.c)
*/

char* display_score(int number){
    return itoaconv(number);
}

void reset()
{
    if(is_reset){
        // Reset starting values for obstacles and BIRD
        p1.x0 = 128;p1.x1 = 130; p1.y0 = 5;  p1.gap = 20; p1.y1 = 25;
        p2.x0 = 256;p2.x1 = 258; p2.y0 = 10; p2.gap = 20; p2.y1 = 30;
        p3.x0 = 384;p3.x1 = 386; p3.y0 = 10; p3.gap = 20; p3.y1 = 30;
        p4.x0 = 512;p4.x1 = 514; p4.y0 = 10; p4.gap = 20; p4.y1 = 30;

        BIRD.x0 = 20; BIRD.x1 = 21; BIRD.y0 = 16; BIRD.y1 = 17; BIRD.velocity = 0; BIRD.acceleration=0;
        pillar_distance = 128; 

        clear_game_field();
        // Reset score and user initials
        (*porte) = 0;
        score = 0;
        strcpy(initials, "AAA");
        // Change gamestate
        is_reset = 0;
    }
}
/* 
    display_end 
    
    After the game ends display_end() will display the user's score
    and change the gamestate to prepare for reset.
*/
void display_end(){
    display_string(0, "Score");
    display_string(1, display_score(score));
    display_update();
    is_reset = 1;
}

/*
    collision_detector()

    Checks if the bird has collided with a pillar or the ground.
    Changes the gamestate to lead to endgame() if it has collided.
*/

void collision_detector(){
    // Collision with pillar 1
    if (((BIRD.x0 <= p1.x0) && (BIRD.x1 >= p1.x0)) && ((BIRD.y0 <= p1.y0) || (BIRD.y1 >= p1.y1 ))) {
        in_game = 0; collide = 1; end_game_start_positioning = 1;
    }
    // Collision with pillar 2
    if (((BIRD.x0 <= p2.x0) && (BIRD.x1 >= p2.x0)) && ((BIRD.y0 <= p2.y0) || (BIRD.y1 >= p2.y1 ))) {
        in_game = 0; collide = 1; end_game_start_positioning = 1;
    }
    // Collision with pillar 3
     if (((BIRD.x0 <= p3.x0) && (BIRD.x1 >= p3.x0)) && ((BIRD.y0 <= p3.y0) || (BIRD.y1 >= p3.y1 ))) {
        in_game = 0; collide = 1; end_game_start_positioning = 1;
    }
    // Collision with pillar 4
    if (((BIRD.x0 <= p4.x0) && (BIRD.x1 >= p4.x0)) && ((BIRD.y0 <= p4.y0) || (BIRD.y1 >= p4.y1 ))) {
        in_game = 0; collide = 1; end_game_start_positioning = 1;
    }
    // Collision with floor
    if (BIRD.y1 >= 31){
        in_game = 0; collide = 1; end_game_start_positioning = 1;
    }
}

/*
    draw_outline()

    Draw a line showing where the floor and ground is.
*/
void draw_outline(){
    pixel(0,0,128,0); // Top line
    pixel(0,31,128,31);
}

/*
    change_difficulty(struct OBSTACLE *pillar)
    
    Alters the pillar's gaps and the distance between pillars as the score increases
*/
void change_difficulty(struct OBSTACLE *pillar){
    if      (score == 3) {  pillar->gap = 14;  pillar_distance = 80;} 
    else if (score == 10){ pillar->gap = 12;  pillar_distance = 60;} 
    else if (score == 15){ pillar->gap = 8;   pillar_distance = 50;} 
    else if (score == 20){ pillar->gap = 6;   pillar_distance = 40;} 
}

/*
    draw_pillar()

    Uses the pixel() function to draw the pillar to the gamefield matrix
*/
void draw_pillar(struct OBSTACLE pillar){
    int yMin = 0;
    int yMax = 31;
    // Upper pillar
    pixel(pillar.x0, yMin, pillar.x1, pillar.y0);
    // Lower pillar
    pixel(pillar.x0, pillar.y1, pillar.x1, yMax);
}

/*
    random_gap_positioning(struct OBSTACLE *pillar)

    Randomly generates where the gap in the newly generated pillar will be
*/
void random_gap_positioning(struct OBSTACLE *pillar){
    int r = random_value(31-(pillar->gap));
    pillar->y0 = r;
    pillar->y1 = r + pillar->gap;
}

/*
    move_pillar(struct OBSTACLE *pillar)

    Moves the given pillar one pixel to the left in the display
    If a pillar has "passed" the screen it will set the new x-value outside the 
    right bound of the screen and create a new randomly generated pillar gap for that pillar
*/

struct OBSTACLE *pillarBefore = &p4;

void move_pillar(struct OBSTACLE *pillar){
    if (pillar->x1 < 0){
        pillar->x0 = pillarBefore->x0 + pillar_distance;
        pillar->x1 = pillar->x0+2;
        random_gap_positioning(pillar);
        pillarBefore = pillar;
    }
        pillar->x0 -= 1;
        pillar->x1 -= 1;
}

/*
    move_player()

    Takes input from the user and changes the bird's velocity, acceleration and positioning based 
    on these structors
*/
void move_player() {
    // JUMP MOVEMENT
    if (get_buttons() & 0x8 && BIRD.velocity >= gravity / 10.0f) {
        BIRD.acceleration = 0;
        BIRD.velocity = -4;
    } else{
        BIRD.acceleration += gravity * ( 1.0f/ 20.0f);
    }
    if (BIRD.acceleration >= gravity){
        BIRD.acceleration = gravity;
    }

    BIRD.velocity += (int) BIRD.acceleration * ( 1.0f/ 20.0f);

    BIRD.y0 += (int) BIRD.velocity * ( 1.0f/ 20.0f);
    BIRD.y1 += (int) BIRD.velocity * ( 1.0f/ 20.0f);
    

    // DASH MOVEMENT
    if(get_buttons() & 0x1 && BIRD.x0==20){
            xvelocity = 25;
    }
    if(BIRD.x0 > 20 && xvelocity >= -5){
            xvelocity -= 20.0f/60.0f;
    } else if (BIRD.x0 < 20){
        BIRD.x0 = 20;
        BIRD.x1 = 21;
        xvelocity = 0;
    }
    BIRD.x0 += xvelocity * (1.0f/20.0f); 
    BIRD.x1 += xvelocity * (1.0f/20.0f);
}

/*
    home_selection_menu()

    Will take button-input to alter the value of global variables, hence affecting what is displayed on the screen (what functions are active) 
*/
void home_selection_menu(void){
    delay(10);
    if(get_buttons() & 0x8 && menu_Selector == 0) {
        in_menu = 0;
        in_game = 1;
        in_end = 0;
    } 
    if(get_buttons() & 0x8 && menu_Selector == 1) {
        in_menu = 0;
        in_game = 0;
        in_end = 0;
        in_highscore = 1;
        highScore_start_positioning = 1;
    }
    if(get_buttons() & 0x2 && menu_Selector != 1) {
        menu_Selector++;
        menu_cursor(menu_Selector);
    }
    if(get_buttons() & 0x4 && menu_Selector != 0) {
        menu_Selector--;
        menu_cursor(menu_Selector);
    }
}

/*
    highscore_selection_menu() 

    Will take button-input to let the user return from entering the highscore sub-menu
*/
void highscore_selection_menu(void) {
    delay(10);
    if(get_buttons() & 0x1) {
        in_menu = 1;
        in_game = 0;
        in_highscore = 0;
        in_end = 0;
        menu_start_positioning = 1;
    }
}

/*
    ascii_increment(char letter)
    
    Increments a given character so that it becomes the next letter in the alphabet
    Function goes from A -> Z and then loops
*/
char ascii_increment(char letter){
    if((int) letter >= 65 && (int) letter < 90) {
        return (char) letter + 1;
    }
    if ((int) letter == 90){
        return (char) 65;
    } else {
        return letter;
    }
}

/*
    end_selection_menu()

    Will take button input to let the user return to the home-screen. 
    Furthermore, it allows the user to change their initials by using the three left-most switches. 
*/
void end_selection_menu(void) {
    delay(1000);
    if(get_buttons() & 0x8) {
        in_game = 0;
        collide = 0;
        menu_Selector = 1;
        in_menu = 1;
        menu_start_positioning = 1;
        in_highscore = 0;
        change_highscore(initials);
        reset();
    } else {
        if(get_switches() & 0x8) {
            initials[0] = ascii_increment(initials[0]);
        }
        if(get_switches() & 0x4) {
            initials[1] = ascii_increment(initials[1]);
        }
        if(get_switches() & 0x2) {
            initials[2] = ascii_increment(initials[2]);
        }
        end_cursor(score, initials);
    }
}

/*
    game_loop()

    the "brain" of the game. 
    The function that is continuously called in the main loop when the game is active. 
*/
void game_loop() {
    if(!collide) {
        clear_string_display();
        pixel(BIRD.x0,BIRD.y0,BIRD.x1,BIRD.y1);
        draw_pillar(p1); 
        draw_pillar(p2); 
        draw_pillar(p3); 
        draw_pillar(p4); 
        draw_outline();
        convert_pixel();
        display_image(0, game_field);
        move_player();
        move_pillar(&p1);
        move_pillar(&p2);
        move_pillar(&p3);
        move_pillar(&p4);
        change_score(&p1);
        change_score(&p2);
        change_score(&p3);
        change_score(&p4);
        collision_detector();
        change_difficulty(&p1);
        change_difficulty(&p2);
        change_difficulty(&p3);
        change_difficulty(&p4);
        clear_game_field();
    }      
}

/* Interrupt Service Routine */
void user_isr( void )
{
    IFSCLR(0) = 0x100;
    if(in_game) {
        game_loop();
    }
}

/* 
    void labinit( void )

    Initialization for LED-lights, TIMER and interrupt settings
*/
void labinit( void )
{
    volatile int *trise = (volatile int*) 0xbf886100;
    TRISECLR = 0xff;    // LEDS
    TRISDSET = 0x0fe0;  //BTNS 2-4
    TRISFSET = 0x2;     //BTNS 1
    // timer stuff
    T2CON = 0x70;
	PR2 = (80000000 / 256) / FPS; 
    TMR2 = 0;
	T2CONSET = (1 << 15);
    IECSET(0) = 0x100;      
	IPCSET(2) = 0x1f;
    clear_game_field();
    enable_interrupt();
}

/* 
    void game_state( void )

    This function is called repetitively from the main program and 
    alters the gamestate of the program, That is: If we are in a menu, in the game
    in the highscore leaderboard etc. The function chooses the gamestate based on the current value
    for a few given variables (in_menu, in_highscore, in_game) etc
*/
void game_state( void )
{
    if (in_menu) {
      home_selection_menu();
      if(menu_start_positioning) {
            clear_string_display();
            start_screen();
            menu_start_positioning = 0;
      }
    }
    else if(in_highscore) {
        highscore_selection_menu();
        if(highScore_start_positioning) {
            clear_string_display();
            display_highscore();
            highScore_start_positioning = 0;
        }
    }
    else if(collide) {
        end_selection_menu();
        if(end_game_start_positioning) {
            clear_string_display();
            display_end();
            end_game_start_positioning = 0;
        }
    }
}