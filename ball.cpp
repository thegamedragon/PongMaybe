#include "ball.h"

//30 x 30

Ball_C::Ball_C()
    : x(100)
    , y(100)
    , width(15)
    , height(15)
    , dx(0)
    , dy(0)
    , speed(12)
{

}

Ball_C::~Ball_C()
{
    //Nothing to destroy yet.
}

void Ball_C::render(SDL_Renderer *renderer,int Screen_Width, int Screen_Height)
{
    update(Screen_Width,Screen_Height);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_Rect ball = {(int)x,(int)y,width,height};
    SDL_RenderFillRect(renderer,&ball);
}

void Ball_C::place_middle(int Width,int Height)
{
    x=Width/2-width/2;
    y=Height/2-height/2;
    dx=0;dy=0;
}

void Ball_C::launch_middle(int Screen_Width,int Screen_Height)
{
    place_middle(Screen_Width,Screen_Height);

    std::srand(SDL_GetTicks());
    direction=(std::rand()%2)==0 ? -1 : 1; //left or right

    angle=std::rand()%121-60;// random -60 to 60

    //I dont want ball to go straight at paddle so
    //Basically angle is [-60;-15] U [15;60].
    if(angle <= 0 && angle > -30) angle-= 35;
    else if(angle > 0 && angle < 30) angle += 35;

    dx = direction*speed*std::cos(angle*M_PI/180.0f);  // speed on the x-axis
    dy =           speed*std::sin(angle*M_PI/180.0f);  // speed on the y-axis

}

void Ball_C::update(int Screen_Width,int Screen_Height)
{
    if( y+dy < 0 || y+height+dy > Screen_Height ) dy-=2*dy;

    x+=dx; //Paddle collision done in pong.cpp ...
    y+=dy;
}

int Ball_C::out_of_bounds(int Screen_Width)
{
    if(x<0) return -1; //Ball went in left-side net
    if(x>Screen_Width) return 1; //Ball went in right-side net
    return 0;
}
