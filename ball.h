#ifndef _BALL_H_
#define _BALL_H_

#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cmath>

class Ball_C
{
public:
    Ball_C();
    ~Ball_C();

    float x,y; //current
    int width,height; //i figured out width=height, but its too late to change now *sigh*
    float dx,dy; //delta for next frame
    int speed; //pixels per sec
    int angle;
    int direction;
    int hits=0;

    void render(SDL_Renderer* renderer,int Screen_Width, int Screen_Height);
    void place_middle(int Width,int Height);
    void launch_middle(int Screen_Width,int Screen_Height);
    void update(int Screen_Width,int Screen_Height);
    int out_of_bounds(int Screen_Width); //-1 if on left side, 1 if on right side
};











#endif // _BALL_H_
