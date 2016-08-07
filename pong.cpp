#include "pong.h"

GameTexture Game::LoadTextTexture(std::string Text)
{
    SDL_Surface* textSurface=TTF_RenderText_Solid(Font,Text.c_str(),WhiteColor);
    GameTexture NewTexture;
    NewTexture.Texture=SDL_CreateTextureFromSurface(renderer,textSurface);
    NewTexture.Height=textSurface->h;
    NewTexture.Width=textSurface->w;
    SDL_FreeSurface(textSurface);
    return NewTexture;
}

Game::Game()
    : Screen_Height(720)
    , Screen_Width(1280)
    , Mouse_x(0)
    , Mouse_y(0)
    , ScoreLeftValue(0)
    , ScoreRightValue(0)
{
    Status=STARTING;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    window=SDL_CreateWindow("Pong Hopefully",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,Screen_Width,Screen_Height,SDL_WINDOW_SHOWN);
    if(window==NULL) printf("WINDOW ERROR\n%s\n\n",SDL_GetError() );

    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer==NULL) printf("RENDERER ERROR\n%s\n\n",SDL_GetError() );

    WhiteColor = {0xFF,0xFF,0xFF, 255};
    BlackColor = {0,0,0,255};

    Font=TTF_OpenFont("Fonts/Quicksand.ttf",44);
    LaunchText = LoadTextTexture("Press SPACE");
    ScoreLeft = LoadTextTexture("0"); //Will change over time
    ScoreRight = LoadTextTexture("0"); //Will change over time
    VictoryLeft = LoadTextTexture("Left VICTORY");
    VictoryRight = LoadTextTexture("Right VICTORY");
    RestartText = LoadTextTexture("Press SPACE to RESTART");

    Ball.place_middle(Screen_Width,Screen_Height);
    Paddle_Player.initialise(Screen_Width,Screen_Height,1);
    Paddle_AI.initialise(Screen_Width,Screen_Height,2);
}

Game::~Game()
{
    SDL_DestroyTexture(LaunchText.Texture);
    SDL_DestroyTexture(ScoreLeft.Texture);
    SDL_DestroyTexture(ScoreRight.Texture);
    SDL_DestroyTexture(VictoryLeft.Texture);
    SDL_DestroyTexture(VictoryRight.Texture);
    SDL_DestroyTexture(RestartText.Texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}

void Game::LetTheGamesBegin()
{
    SDL_Event EventQueue;
    int time;
    bool quit=false;
    Status=READY_TO_LAUNCH;

    SDL_Rect Rect_ScoreLeft={150,50,ScoreLeft.Width,ScoreLeft.Height};
    SDL_Rect Rect_ScoreRight={Screen_Width-150-ScoreRight.Width,50,ScoreRight.Width,ScoreRight.Height};
    SDL_Rect Rect_VictoryLeft={Screen_Width/2-VictoryLeft.Width/2,100,VictoryLeft.Width,VictoryLeft.Height};
    SDL_Rect Rect_VictoryRight={Screen_Width/2-VictoryRight.Width/2,100,VictoryRight.Width,VictoryRight.Height};
    SDL_Rect Rect_LaunchText={Screen_Width/2-LaunchText.Width/2,100,LaunchText.Width,LaunchText.Height};
    SDL_Rect Rect_RestartText={ Screen_Width/2-RestartText.Width/2, 120+VictoryLeft.Height ,RestartText.Width,RestartText.Height};

    while(!quit)
    {
        time = SDL_GetTicks();
        while(SDL_PollEvent(&EventQueue))
        {
            switch(EventQueue.type)
            {
            case SDL_QUIT:
                quit=true;
                continue;
            case SDL_MOUSEMOTION:
                Mouse_x=EventQueue.motion.x;
                Mouse_y=EventQueue.motion.y;
                continue;
            case SDL_KEYDOWN:
                if(EventQueue.key.keysym.sym==SDLK_SPACE)
                {
                    if(Status==READY_TO_LAUNCH)
                    {Ball.launch_middle(Screen_Width,Screen_Height);Status=PLAYING;}
                    else if (Status==VICTORY_LEFT || Status==VICTORY_RIGHT)
                    {
                        Reset_Game();
                        Status=READY_TO_LAUNCH;
                    }
                }
                continue;
            default:
                continue;
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        //All texts
        SDL_RenderCopy(renderer,ScoreLeft.Texture,NULL,&Rect_ScoreLeft);
        SDL_RenderCopy(renderer,ScoreRight.Texture,NULL,&Rect_ScoreRight);
        if(Status==READY_TO_LAUNCH) SDL_RenderCopy(renderer,LaunchText.Texture,NULL,&Rect_LaunchText);
        if(Status==VICTORY_LEFT)
        {
            SDL_RenderCopy(renderer,VictoryLeft.Texture,NULL,&Rect_VictoryLeft);
            SDL_RenderCopy(renderer,RestartText.Texture,NULL,&Rect_RestartText);
        }
        if(Status==VICTORY_RIGHT)
        {
            SDL_RenderCopy(renderer,VictoryRight.Texture,NULL,&Rect_VictoryRight);
            SDL_RenderCopy(renderer,RestartText.Texture,NULL,&Rect_RestartText);
        }
        ///TODO ADD RESET TEXT.

        Paddle_collision();
        Scoring();

        Ball.render(renderer,Screen_Width,Screen_Height);
        Paddle_Player.render(renderer,Screen_Width,Screen_Height,Mouse_y,(int) Ball.y+Ball.height/2);
        Paddle_AI.render(renderer,Screen_Width,Screen_Height,Mouse_y,(int) Ball.y+Ball.height/2);

        SDL_RenderPresent(renderer);

        //Sleep (cap fps at 60) - must be at the end of the while
        time = SDL_GetTicks() - time;
        if(time < 0) continue; //if time negative, continure rendering asap
        time= 20 - time;
        if(time > 0 ) SDL_Delay(time);

    }

}

void Game::Paddle_collision() //GOTTA REDO, IT IS A MESS, BUT SOMEWHAT FUNCTIONAL
{
    //if next frame collision with either paddle
    //~~~move the ball on the surface~~nah~~ then change direction
    if(Ball.x >= Paddle_Player.x + Paddle_Player.width)
    if(Ball.x+Ball.dx <= Paddle_Player.x + Paddle_Player.width && (Ball.y+Ball.height/2+Ball.dy >= Paddle_Player.y + Paddle_Player.dy && Ball.y+Ball.height/2+Ball.dy <= Paddle_Player.y + Paddle_Player.height + Paddle_Player.dy) )
    {
        Ball.direction=-Ball.direction;
        Ball.dx = Ball.direction*Ball.speed*std::cos(Ball.angle*M_PI/180.0f);
        if(++Ball.hits >= 4) {Ball.hits=0;Ball.speed+=2;}
    }

    if(Ball.x <= Paddle_AI.x)
    if(Ball.x+Ball.dx >= Paddle_AI.x && ( Ball.y+Ball.height/2+Ball.dy >= Paddle_AI.y + Paddle_AI.dy && Ball.y+Ball.height/2+Ball.dy <= Paddle_AI.y +Paddle_AI.height+ Paddle_AI.dy ) )
    {
        Ball.direction=-Ball.direction;
        Ball.dx = Ball.direction*Ball.speed*std::cos(Ball.angle*M_PI/180.0f);
        if(++Ball.hits >= 4) {Ball.hits=0;Ball.speed+=2;}
    }
}

void Game::Scoring()
{
    char *SCORE=new char[5];

    if(Ball.out_of_bounds(Screen_Width)==-1)
    {
        ScoreRightValue++;
        SDL_DestroyTexture(ScoreRight.Texture);
        sprintf(SCORE,"%d",ScoreRightValue);
        ScoreRight=LoadTextTexture(SCORE);
        Status=READY_TO_LAUNCH;
        Ball.place_middle(Screen_Width,Screen_Height);
        Ball.hits=0;
        Ball.speed=12;
    }
    else if(Ball.out_of_bounds(Screen_Width)==1)
    {
        ScoreLeftValue++;
        SDL_DestroyTexture(ScoreLeft.Texture);
        sprintf(SCORE,"%d",ScoreLeftValue);
        ScoreLeft=LoadTextTexture(SCORE);
        Status=READY_TO_LAUNCH;
        Ball.place_middle(Screen_Width,Screen_Height);
        Ball.hits=0;
        Ball.speed=12;
    }
    delete[] SCORE;

    if(ScoreLeftValue >= SCORE_LIMIT ) Status=VICTORY_LEFT;
    else if(ScoreRightValue >= SCORE_LIMIT) Status=VICTORY_RIGHT;
}

void Game::Reset_Game()
{
    Ball.dx=0;
    Ball.dy=0;
    Ball.hits=0;
    Ball.speed=12;
    Ball.place_middle(Screen_Width,Screen_Height);

    SDL_DestroyTexture(ScoreRight.Texture);
    SDL_DestroyTexture(ScoreLeft.Texture);

    ScoreLeftValue=ScoreRightValue=0;

    ScoreLeft=LoadTextTexture("0");
    ScoreRight=LoadTextTexture("0");

}


