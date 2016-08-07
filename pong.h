#ifndef _PONG_H_
#define _PONG_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include "ball.h"
#include "paddle.h"

#define SCORE_LIMIT 3

struct GameTexture
{
    SDL_Texture* Texture;
    int Width;
    int Height;
};

class Game
{
private:
    GameTexture LoadTextTexture(std::string Text); //Color is implicit white

public:
    Game(); //Initialize, Load all the media files
    ~Game(); //Quit, destroy everything

    enum Game_Status {STARTING,READY_TO_LAUNCH,PLAYING,VICTORY_LEFT,VICTORY_RIGHT};
    Game_Status Status;

    int Screen_Height;
    int Screen_Width;

    //Window the game will run + the renderer
    SDL_Window *window;
    SDL_Renderer *renderer;

    //All are Test
    GameTexture LaunchText;
    GameTexture ScoreLeft;
    GameTexture ScoreRight;
    GameTexture VictoryLeft;
    GameTexture VictoryRight;
    GameTexture RestartText;
    TTF_Font *Font;

    //For paddle rendering
    SDL_Color WhiteColor;
    SDL_Color BlackColor;

    //Game Objects
    Ball_C Ball;
    Paddle_C Paddle_Player;
    Paddle_C Paddle_AI;

    //Mouse place
    int Mouse_x,Mouse_y; //Changes on Event

    //Score
    int ScoreLeftValue;
    int ScoreRightValue;

    //Cycle
    void LetTheGamesBegin(); //will handle all input

    //Auxiliary
    void Paddle_collision();
    //^ I cant come up with a better idea of solving this (due to needing access to both the ball and paddle..), thus this shall be a game-class function
    void Scoring();
    void Reset_Game();
};











#endif // _PONG_H_

