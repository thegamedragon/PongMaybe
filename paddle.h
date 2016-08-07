#ifndef _PADDLE_H_
#define _PADDLE_H_

#include <SDL.h>
#include <stdio.h>
#include <string>

class Paddle_C
{
public:
    Paddle_C();
    ~Paddle_C();

    enum Paddle_C_Type {PLAYER=1,AI=2};
    Paddle_C_Type type;

    int dy;
    int speed;
    int x,y;
    int width,height;

    void initialise(int Screen_Width,int Screen_Height, int type); //1=player, 2=ai
    void render(SDL_Renderer* renderer, int Screen_Width, int Screen_Height, int Mouse_y, int Ball_y);
    void update(int Screen_Width, int Screen_Height);
    void move_paddle(int Mouse_y,int Ball_y);
};











#endif // _PADDLE_H_

