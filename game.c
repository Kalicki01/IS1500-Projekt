    #include <stdio.h>   
    #include <stdlib.h>
    #include <stdbool.h>
    #include <unistd.h>

    int FPS = 1;
    struct BIRD{
        float x,y;
        float velocity;
        float acceleration;
    };

    bool collision(struct BIRD *BIRD){
        if(BIRD->y > 80 | BIRD->y<1){
            return true;
        }
        return false;
    }

    void updateBirdPosition(bool userInput, struct BIRD *BIRD){
        if(userInput){
            BIRD->velocity = 30;
        }
        BIRD->velocity = BIRD->velocity + (BIRD->acceleration*1/FPS);
        BIRD->y=BIRD->y + (BIRD->velocity * 1/FPS);
    }

    bool checkInput(){
        int r = rand() % 5;
        if (r == 1){
            return true;
        } else{
            return false;
        }
    }

    void draw(struct BIRD *BIRD){
        // 128 x 32
        int y = (int) BIRD->y;
        int x = (int) BIRD->x;
        printf("----------------------------------------------");
        for(int i=0; i < (80-y);i++){
            printf("\n");
        }
        for(int i=0;i < x; i++){
            printf(" ");
        }
        printf("X");
        for(int i=0;i<y; i++){
            printf("\n");
        }
    }

    int startGame(struct BIRD *BIRD){
        bool userInput;
        while(!collision(BIRD)){
            updateBirdPosition(userInput, BIRD);
            userInput = checkInput();
            draw(BIRD);
            sleep(1/FPS);
        }
    }

    int main(){    

        struct BIRD BIRD;
        // set the starting points for the bird
        BIRD.x=30;
        BIRD.y=50;
        BIRD.velocity=0;
        BIRD.acceleration= -9.81;

        startGame(&BIRD);
    }  

