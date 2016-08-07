#include "paddle.h"

// 30width, 200 height
// 1/50 width, 1/9 height

Paddle_C::Paddle_C()
    : speed(16)
{

}

void Paddle_C::initialise(int Screen_Width,int Screen_Height, int type)
{
    width=1.0f/50.0f*Screen_Width;
    height=1.0f/7.0f*Screen_Height;
    y=Screen_Height/2.0f-1.0f/2.0f*height;

    if(type==PLAYER)
    {
        this->type=PLAYER;
        x=10;
    }
    else
    {
        this->type=AI;
        x=Screen_Width-10-width;
    }

}

Paddle_C::~Paddle_C()
{
    //Nothing to destroy yet
}

void Paddle_C::move_paddle(int Mouse_y,int Ball_y)
{
    if(type==PLAYER)
    {
        Mouse_y-=height/2;
        if( Mouse_y < y )
        {
            if( y-Mouse_y > speed ) dy=-speed;
            else dy=Mouse_y-y;
        }
        else if( Mouse_y > y )
        {
            if(Mouse_y-y > speed ) dy=speed;
            else dy=Mouse_y-y;
        }
        else if (Mouse_y == y) dy=0;
    }
    else if(type==AI)
    {
        Ball_y-=height/2;
        if( Ball_y < y )
        {
            if( y-Ball_y > speed ) dy=-speed;
            else dy=Ball_y-y;
        }
        else if( Ball_y > y )
        {
            if(Ball_y-y > speed ) dy=speed;
            else dy=Ball_y-y;
        }
        else if (Ball_y == y) dy=0;
    }
}

void Paddle_C::render(SDL_Renderer* renderer, int Screen_Width,int Screen_Height, int Mouse_y, int Ball_y)
{
    move_paddle(Mouse_y,Ball_y);
    update(Screen_Width,Screen_Height);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_Rect Paddle = {x,y,width,height};
    SDL_RenderFillRect(renderer,&Paddle);
}

void Paddle_C::update(int Screen_Width, int Screen_Height)
{
    y+=dy;
    if(y<=0) y=0;
    else if(y>=Screen_Height-height) y=Screen_Height-height;
}
