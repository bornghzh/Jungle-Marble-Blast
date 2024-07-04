#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <SDL2/SDL_mixer.h>
using namespace std;
SDL_Window *m_window = SDL_CreateWindow("proje", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 800, SDL_WINDOW_SHOWN);
SDL_Renderer *m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
int W,H;
class Timer{
    Uint32 startTicks, pauseTicks;
    bool paused, started;

public:
    Timer(){
        startTicks = 0;
        pauseTicks = 0;

        paused = false;
        started = false;
    }

    void start(){
        startTicks = SDL_GetTicks();
        pauseTicks = 0;
        paused = false;
        started = true;
    }
    void restart() { start(); }
    void stop(){
        started = false;

        //Unpause the timer
        paused = false;

	    //Clear tick variables
	    startTicks = 0;
	    pauseTicks = 0;
    }
    void pause() {
        if( started && !paused ) {
            //Pause the timer
            paused = true;

            //Calculate the paused ticks
            pauseTicks = SDL_GetTicks() - startTicks;
		    startTicks = 0;
        }
    }
    void unpause() {
        if( started && paused ) {
            //Unpause the timer
            paused = false;

            //Reset the starting ticks
            startTicks = SDL_GetTicks() - pauseTicks;

            //Reset the paused ticks
            pauseTicks = 0;
        }
    }
    Uint32 getTicks(){
        Uint32 time = 0;
        if( started ) {
            //If the timer is paused
            if( paused ) {
                //Return the number of ticks when the timer was paused
                time = pauseTicks;
            }
            else {
                //Return the current time minus the start time
                time = SDL_GetTicks() - startTicks;
            }
        }
        return time;
    }
};
class image
{
private:
    SDL_Renderer *ren;
    SDL_Rect img_rect;
public:
    string path;
    SDL_Texture *img;
    int img_w, img_h;
    int x;
    int y;
    float sc_x;
    float sc_y;
    void run()
    {
        img = IMG_LoadTexture(ren, path.c_str());
        SDL_QueryTexture(img, NULL, NULL, &img_w, &img_h);
    }

    void load()
    {
        //const char* cpath = path.c_str();


        img_rect.x = x;
        img_rect.y = y;
        img_rect.w = img_w * sc_x;
        img_rect.h = img_h * sc_y;
        SDL_RenderCopy(ren, img, NULL, &img_rect);
        //delete cpath;
    }
    image(string Path, int X, int Y, float SC, SDL_Renderer *Ren)
    {
        x = X;
        y = Y;
        path = Path;
        sc_x = SC;
        sc_y = SC;
        ren = Ren;
        run();
        load();
    }
    void setCenter(int X, int Y)
    {
        x = X - img_w  * sc_x / 2;
        y = Y - img_h * sc_y / 2;
        load();
    }
    void render()
    {
        SDL_RenderCopy(ren, img, NULL, &img_rect);
    }
    ~image()
    {
        SDL_DestroyTexture(img);
    }
};
class btn
{
private:
    int x;
    int y;
    int he;
    int wi;
public:
    image *pic;
    btn(int X, int Y, string Path, float SC,SDL_Renderer *Ren)
    {
        pic = new image(Path, X, Y, SC, Ren);
        x = X;
        y = Y;
        he = pic->img_h;
        wi = pic->img_w;
    }
    ~btn()
    {
        delete pic;
    }
    void setXY(int X, int Y)
    {
        x = X;
        y = Y;
        pic->x = X;
        pic->y = Y;
        pic->load();
    }
    void setCenter(int X, int Y)
    {
        x = X - wi * pic->sc_x / 2;
        y = Y - he * pic->sc_y / 2;
        pic->x = x;
        pic->y = y;
        pic->load();
    }
    bool btn_clicked(int mouse_x, int mouse_y)
    {
        if(mouse_x >= x && mouse_x <= x + wi * pic->sc_x
                && mouse_y >= y && mouse_y <= y + he * pic->sc_y)
            return true;
        else
            return false;
    }

};
struct Point{
    int x,y;
};
struct player{
    string usme,psrd;
    int score;
};
struct Ball
{
    Point makan;
    double dx,dy;
    char cl;
    bool vaziat = true;
    int type = 0;//1 : fireball  2:bomb
};


int score = 0;
vector <Ball> balls;
vector <Ball> balls_comming;
vector <char> color;
vector <player> players;
int num_ball = 0,num_ball_comming = 0;
int num_ball_comming_cnst = 0;
image *backg = new image("2.png",0,0,1.0, m_renderer);
image *help = new image("help.png",360,300,0.3,m_renderer);
image *im1 = new image("background1.png",0,0,1.0,m_renderer);
image *im2 = new image("background2.png",0,0,1.0,m_renderer);
image *canon = new image("canon.png",100,300,0.4,m_renderer);
image *pause = new image("pause.png",340,144,1.0,m_renderer);
btn *login_btn1 = new btn(545,395,"login1.png",0.5,m_renderer);
btn *login_btn2 = new btn(545,395,"login2.png",0.5,m_renderer);
btn *start_btn1 = new btn(545,365,"start1.png",0.5,m_renderer);
btn *start_btn2 = new btn(545,365,"start2.png",0.5,m_renderer);
btn *setting_btn1 = new btn(545,525,"setting1.png",0.5,m_renderer);
btn *setting_btn2 = new btn(545,525,"setting2.png",0.5,m_renderer);
btn *register_btn1 = new btn(545,495,"register1.png",0.5,m_renderer);
btn *register_btn2 = new btn(545,495,"register2.png",0.5,m_renderer);
btn *music_btn1 = new btn(545,395,"music1.png",0.5,m_renderer);
btn *music_btn2 = new btn(545,395,"music2.png",0.5,m_renderer);
btn *back_btn1 = new btn(100,600,"back1.png",0.5,m_renderer);
btn *back_btn2 = new btn(100,600,"back.png",0.5,m_renderer);
btn *help_btn1 = new btn(515,465,"help1.png",0.67,m_renderer);
btn *help_btn2 = new btn(515,465,"help2.png",0.67,m_renderer);
btn *username_btn = new btn(135,300,"username.png",1.0,m_renderer);
btn *password_btn = new btn(135,400,"Password.png",1.0,m_renderer);
btn *save_btn1 = new btn(980,600,"save1.png",0.5,m_renderer);
btn *save_btn2 = new btn(980,600,"save2.png",0.5,m_renderer);
btn *ok_btn1 = new btn(550,650,"ok1.png",0.5,m_renderer);
btn *ok_btn2 = new btn(550,650,"ok2.png",0.5,m_renderer);
btn *leaderboard_btn1 = new btn(515,435,"leaderboard1.png",0.67,m_renderer);
btn *leaderboard_btn2 = new btn(515,435,"leaderboard2.png",0.67,m_renderer);
btn *music1_btn1 = new btn(545,365,"music11.png",0.5,m_renderer);
btn *music1_btn2 = new btn(545,365,"music12.png",0.5,m_renderer);
btn *music2_btn1 = new btn(545,445,"music21.png",0.5,m_renderer);
btn *music2_btn2 = new btn(545,445,"music22.png",0.5,m_renderer);
btn *music3_btn1 = new btn(545,525,"music32.png",0.5,m_renderer);
btn *music3_btn2 = new btn(545,525,"music31.png",0.5,m_renderer);
btn *mute_btn1 = new btn(545,605,"mute1.png",0.5,m_renderer);
btn *mute_btn2 = new btn(545,605,"mute2.png",0.5,m_renderer);
btn *map1_btn1 = new btn(545,365,"map1.png",0.5,m_renderer);
btn *map1_btn2 = new btn(545,365,"map12.png",0.5,m_renderer);
btn *map2_btn1 = new btn(545,525,"map21.png",0.5,m_renderer);
btn *map2_btn2 = new btn(545,525,"map22.png",0.5,m_renderer);
Mix_Music *background;
Mix_Chunk *jumpeffect;
Mix_Chunk *enfejar;
SDL_Event *e = new SDL_Event();
Point masir[68];
Point masir2[47];


void charkhesh(SDL_Renderer *m_renderer,int x,int y,int w,int h,double degree,string address)
{
    degree *= 180.0/M_PI;
    SDL_Texture *mTexture = IMG_LoadTexture(m_renderer,address.c_str());
    SDL_Rect  rectangle = {x,y,w,h};
    SDL_QueryTexture(mTexture,NULL,NULL,&w,&h);
    SDL_RenderCopyEx(m_renderer,mTexture,NULL,&rectangle,degree,NULL,SDL_FLIP_NONE);
    SDL_DestroyTexture(mTexture);
}
void betterText(SDL_Renderer *renderer, string S,int x, int y,int r, int g, int b, int a,int size, string Font)
{
    TTF_Font *font = TTF_OpenFont(Font.c_str(), size);
    SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(font, S.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;
    int h, w;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    rect.h = h;
    rect.w = w;
    rect.x = x ;
    rect.y = y ;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
bool colapse(Ball A,Ball B)
{
    if((A.makan.x - B.makan.x) * (A.makan.x - B.makan.x) + (A.makan.y - B.makan.y) * (A.makan.y - B.makan.y) <= 4901)
        return true;
    return false;
}
int bisharaf(int a)
{
    int counter = 0;
    bool chek_left = true;
    bool chek_right = true;
    int num_left = 0;
    int num_right = 0;
    while(chek_left)
    {
        num_left++;
        if(balls_comming[a].cl == balls_comming[a - num_left].cl){
            counter++;
        }
        else{
            chek_left = false;
        }
    }
    while(chek_right)
    {
        num_right++;
        if(balls_comming[a].cl == balls_comming[a + num_right].cl){
            counter++;
        }
        else{
            chek_right = false;
        }
    }
    return counter;
}
void fucking_game(int a)
{
    num_ball_comming -= bisharaf(a) + 1;
    bool chek_left = true;
    bool chek_right = true;
    int num_left = 0;
    int num_right = 0;
    while(chek_left)
    {
        if(balls_comming[a].cl == balls_comming[a - num_left - 1].cl)
            num_left++;
        else
            chek_left = false;
    }
    while(chek_right)
    {
        if(balls_comming[a].cl == balls_comming[a + num_right + 1].cl)
            num_right++;
        else
            chek_right = false;
    }
    for(int i=0;i<=num_left+num_right;i++)
    {
        auto element = balls_comming.begin() + a - num_left;
        balls_comming.erase(element);
    }
    score += num_left + num_right + 1;
}
void fireb(int a)
{
    auto element = balls_comming.begin() + a;
    balls_comming.erase(element);
    num_ball_comming--;
    score++;
}
void multi_color(char clr)
{
    int ar[num_ball_comming],cnt = 0;
    for(int i=0;i<num_ball_comming;i++)
    {
        if(balls_comming[i].cl == clr)
            ar[cnt++] = i;
    }
    for(int i=cnt-1;i>=0;i--)
    {
        auto element = balls_comming.begin() + ar[i];
        balls_comming.erase(element);
    }
    num_ball_comming -= cnt;
    score+=cnt;
}
void bomb(int a)
{
    if(num_ball_comming>=9){
    if(a-4>=0)
    {
        for(int i=0;i<9;i++)
        {
            auto elmt = balls_comming.begin() + a - 4;
            balls_comming.erase(elmt);
        }
    }else
    {
        if(a < 4)
        {
            for(int i=0;i<9;i++)
            {
                auto elmt = balls_comming.begin();
                balls_comming.erase(elmt);
            }
        }
        else
        {
            for(int i=0;i<9;i++)
            {
                auto elmt = balls_comming.end();
                balls_comming.erase(elmt);
            }
        }
    }
    num_ball_comming -= 9;
    score+=9;
    }else
    {
        for(int i=0;i<num_ball_comming;i++)
        {
            auto elmt = balls_comming.begin();
            balls_comming.erase(elmt);
        }
        score+=num_ball_comming;
        num_ball_comming = 0;
    }
}
void game()
{
    im1->render();
    charkhesh(m_renderer,505,160,270,480,M_PI/3,"canon.png");
    Point center;
    double degree;
    center.x = 640;
    center.y = 400;
    char colors[4] = {'r','g','b','y'};
    double v = 30;
    color.push_back(colors[rand()%4]);
    color.push_back(colors[rand()%4]);
    bool chekcker  = true;
    bool checker_key_pause = false;
    SDL_RenderPresent(m_renderer);
    bool running = true;
    Timer timer;
    Timer timer2;
    timer.start();
    timer2.start();
    bool enter = true;
    bool checker_key = false;
    int time = 80;
    int cnt = 0;
    int powers[3] = {5,5,5};
    while(running)
    {
        SDL_RenderClear(m_renderer);
        im1->render();
        filledCircleRGBA(m_renderer,50,650,20,250,107,15,255);
        filledCircleRGBA(m_renderer,50,700,20,250,15,207,255);
        filledCircleRGBA(m_renderer,50,750,20,0,0,0,255);
        SDL_PollEvent(e);
        SDL_ShowCursor(SDL_ENABLE);
        int x = e->button.x;
        int y = e->button.y;
        degree = acos((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
        degree += M_PI/2;
        if(y < center.y){
            degree *= -1;
            degree += M_PI;
        }
        aalineRGBA(m_renderer,center.x,center.y,x,y,255,255,255,255);
        charkhesh(m_renderer,505,160,270,480,degree,"canon.png");
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_f && checker_key)
        {
            char clr = 'f';
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            powers[0] -= 1;
            //num_ball+=2;
            checker_key = false;
        }
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p && checker_key)
        {
            timer.pause();
            timer2.pause();
            checker_key_pause = true;
            checker_key = false;
        }
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_r && checker_key)
        {
            timer.unpause();
            timer2.unpause();
            checker_key_pause = false;
            checker_key = false;
        }
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_m && checker_key)
        {
            char clr = 'm';
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            powers[2] -= 1;
            //num_ball+=2;
            checker_key = false;
        }
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b && checker_key)
        {
            char clr = 't';
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            powers[1] -= 1;
            //num_ball+=2;
            checker_key = false;
        }
        if(e->type == SDL_KEYUP)
        {
            checker_key = true;
        }
        switch(color[num_ball])
        {
            case 'b':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,0,0,0,255);
                break;
            case 'e':
                break;
        }
        switch(color[num_ball + 1])
            {
            case 'b':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,0,0,0,255);
                break;
            case 'e':
                break;
            }
            if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE)
        {
            swap(color[num_ball],color[num_ball+1]);
            switch(color[num_ball])
            {
            case 'b':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,0,0,0,255);
                break;
            case 'e':
                break;
        }
        switch(color[num_ball + 1])
        {
            case 'b':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,0,0,0,255);
                break;
            case 'e':
                break;
        }
        }
        //SDL_RenderPresent(m_renderer);
        if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT && chekcker && !checker_key_pause)
        {
            Mix_PlayChannel(-1,enfejar,0);
            double dx1 = (x - center.x)/(sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
            double dy1 = (y - center.y)/(sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
            char cl = colors[rand()%4];
            Ball tmp;
            tmp.makan.x = 640 + (80) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
            tmp.makan.y = 400 + (80) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
            tmp.cl = color[num_ball];
            tmp.dx = dx1;
            tmp.dy = dy1;
            balls.push_back(tmp);
            chekcker = false;
            num_ball++;
            color.push_back(cl);
        }
        if(e->type = SDL_MOUSEBUTTONUP)
        {
            chekcker = true;
        }
        for(int i=0;i<num_ball;i++)
        {
            if(balls[i].type == 0){
            switch(balls[i].cl)
            {
            case 'b':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,0,0,0,255);
                break;
            case 'e':
                break;
            }
            }
            //SDL_RenderPresent(m_renderer);
            if(balls[i].vaziat)
            {
                balls[i].makan.x += balls[i].dx * v;
                balls[i].makan.y += balls[i].dy * v;
            }
            for(int j=0;j<num_ball_comming && num_ball_comming != 0;j++)
            {
                if(colapse(balls[i],balls_comming[j]))
                {
                    if(balls[i].cl == 'f')
                    {
                        balls[i].vaziat = false;
                    auto element1 = balls.begin() + i;
                    auto element2 = color.begin() + i;
                    balls.erase(element1);
                    color.erase(element2);
                    num_ball--;
                        fireb(j);
                    }else if(balls[i].cl == 'm')
                    {
                        balls[i].vaziat = false;
                        auto element1 = balls.begin() + i;
                        auto element2 = color.begin() + i;
                        balls.erase(element1);
                        color.erase(element2);
                        num_ball--;
                        switch(balls_comming[j].cl)
                        {
                        case 'b':
                            multi_color('b');
                            break;
                        case 'g':
                            multi_color('g');
                            break;
                        case 'r':
                            multi_color('r');
                            break;
                        case 'y':
                            multi_color('y');
                            break;
                        }
                    }
                    else if(balls[i].cl == 't')
                    {
                        balls[i].vaziat = false;
                        auto element1 = balls.begin() + i;
                        auto element2 = color.begin() + i;
                        balls.erase(element1);
                        color.erase(element2);
                        num_ball--;
                        bomb(j);
                    }
                    else
                    {
                    balls_comming.insert(balls_comming.begin() + j,balls[i]);
                    balls[i].vaziat = false;
                    auto element1 = balls.begin() + i;
                    auto element2 = color.begin() + i;
                    balls.erase(element1);
                    color.erase(element2);
                    num_ball--;
                        if(bisharaf(j) >= 2)
                        {
                            fucking_game(j);
                        }
                        else{
                            num_ball_comming++;
                        }
                    }

//                    while(bisharaf(j) >= 2)
//                    {
//                        fucking_game(j);
//                    }
                    break;
                }
            }
            if(balls[i].makan.x  > W || balls[i].makan.x  < 0 || balls[i].makan.y  > H || balls[i].makan.y < 0)
            {
                auto element1 = balls.begin() + i;
                auto element2 = color.begin() + i;
                balls.erase(element1);
                color.erase(element2);
                num_ball--;
            }
        }
        for(int i=num_ball_comming;i>=0 && num_ball_comming != 0 ;i--)
        {
                switch(balls_comming[num_ball_comming- i].cl)
                {
                case 'b':
                    balls_comming[num_ball_comming- i].makan.x = masir[i+cnt].x;
                    balls_comming[num_ball_comming- i].makan.y = masir[i+cnt].y;
                    filledCircleRGBA(m_renderer,masir[i+cnt].x,masir[i+cnt].y,35,75,68,193,255);
                    break;
                case 'r':
                     balls_comming[num_ball_comming- i].makan.x = masir[i+cnt].x;
                    balls_comming[num_ball_comming- i].makan.y = masir[i+cnt].y;
                    filledCircleRGBA(m_renderer,masir[i+cnt].x,masir[i+cnt].y,35,232,23,31,255);
                    break;
                case 'y':
                    balls_comming[num_ball_comming- i].makan.x = masir[i+cnt].x;
                    balls_comming[num_ball_comming- i].makan.y = masir[i+cnt].y;
                    filledCircleRGBA(m_renderer,masir[i+cnt].x,masir[i+cnt].y,35,241,216,14,255);
                    break;
                case 'g':
                    balls_comming[num_ball_comming- i].makan.x = masir[i+cnt].x;
                    balls_comming[num_ball_comming- i].makan.y = masir[i+cnt].y;
                    filledCircleRGBA(m_renderer,masir[i+cnt].x,masir[i+cnt].y,35,72,222,18,255);
                    break;
                }
        }
        if(timer.getTicks() >= 1000 && timer2.getTicks()<=81000 && !checker_key_pause)
        {
            Ball tmp;
            char cl = colors[rand()%4];
            tmp.cl = cl;
            tmp.dx=0;
            tmp.dy = 0;
            tmp.makan.x = 0;
            tmp.makan.y = 0;
            tmp.vaziat = true;
            balls_comming.push_back(tmp);
            num_ball_comming++;
            num_ball_comming_cnst++;
            time--;
            timer.restart();
        }
        if(timer2.getTicks() >= 80000 && timer.getTicks()>=1000 && !checker_key_pause)
        {
            timer.restart();
            cnt++;
        }
        betterText(m_renderer,to_string(time),50,10,255,255,255,255,25,"@AlloFont(Airfool).otf");
        betterText(m_renderer,to_string(score),10,10,255,255,255,255,25,"@AlloFont(Airfool).otf");
        if(num_ball_comming>66 )
        {

            betterText(m_renderer,"Game Over!!!!",300,400,232,23,31,255,70,"@AlloFont(Airfool).otf");
             SDL_RenderPresent(m_renderer);
            break;
        }
        if(num_ball_comming + cnt > 66)
        {
            betterText(m_renderer,"Game Over",300,400,232,23,31,255,70,"@AlloFont(Airfool).otf");
            SDL_RenderPresent(m_renderer);
            break;
        }
        betterText(m_renderer,to_string(powers[0]),40,640,255,255,255,255,25,"@AlloFont(Airfool).otf");
        betterText(m_renderer,to_string(powers[2]),40,690,255,255,255,255,25,"@AlloFont(Airfool).otf");
        betterText(m_renderer,to_string(powers[1]),40,740,255,255,255,255,25,"@AlloFont(Airfool).otf");
        if(num_ball_comming <= 0 && num_ball_comming_cnst >= 80)
        {
            im1->render();
            betterText(m_renderer,"You won !!!!!",400,350,72,222,18,255,70,"@AlloFont(Airfool).otf");
            SDL_RenderPresent(m_renderer);
            break;
        }
        if(checker_key_pause)
        {
            pause->render();
        }
        SDL_RenderPresent(m_renderer);
    }
}
void game2()
{
    im2->render();
    charkhesh(m_renderer,505,160,270,480,M_PI/3,"canon.png");
    Point center;
    double degree;
    center.x = 640;
    center.y = 400;
    char colors[4] = {'r','g','b','y'};
    double v = 30;
    color.push_back(colors[rand()%4]);
    color.push_back(colors[rand()%4]);
    bool chekcker  = true;
    bool chekcker_key_pause = false;
    SDL_RenderPresent(m_renderer);
    bool running = true;
    Timer timer;
    Timer timer2;
    timer.start();
    timer2.start();
    bool enter = true;
    bool checker_key = false;
    int time = 80;
    int cnt = 0;
    int powers[3] = {5,5,5};
    while(running)
    {
        SDL_RenderClear(m_renderer);
        im2->render();
        filledCircleRGBA(m_renderer,50,650,20,250,107,15,255);
        filledCircleRGBA(m_renderer,50,700,20,250,15,207,255);
        filledCircleRGBA(m_renderer,50,750,20,0,0,0,255);
        SDL_PollEvent(e);
        SDL_ShowCursor(SDL_ENABLE);
        int x = e->button.x;
        int y = e->button.y;
        degree = acos((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
        degree += M_PI/2;
        if(y < center.y){
            degree *= -1;
            degree += M_PI;
        }
        aalineRGBA(m_renderer,center.x,center.y,x,y,255,255,255,255);
        charkhesh(m_renderer,505,160,270,480,degree,"canon.png");
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_f && checker_key)
        {
            char clr = 'f';
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            powers[0] -= 1;
            //num_ball+=2;
            checker_key = false;
        }
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p && checker_key)
        {
            timer.pause();
            timer2.pause();
            chekcker_key_pause = true;
            checker_key = false;
        }
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_r && checker_key)
        {
            timer.unpause();
            timer2.unpause();
            chekcker_key_pause = false;
            checker_key = false;
        }
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_m && checker_key)
        {
            char clr = 'm';
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            powers[2] -= 1;
            //num_ball+=2;
            checker_key = false;
        }
        if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b && checker_key)
        {
            char clr = 't';
            color.insert(color.end() - 2 - 3 * (5 - powers[0]) - (5 - powers[1]) - (5 - powers[2]),clr);
            powers[1] -= 1;
            //num_ball+=2;
            checker_key = false;
        }
        if(e->type == SDL_KEYUP)
        {
            checker_key = true;
        }
        switch(color[num_ball])
        {
            case 'b':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,0,0,0,255);
                break;
            case 'e':
                break;
        }
        switch(color[num_ball + 1])
            {
            case 'b':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,0,0,0,255);
                break;
            case 'e':
                break;
            }
            if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE)
        {
            swap(color[num_ball],color[num_ball+1]);
            switch(color[num_ball])
            {
            case 'b':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,640 + (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 + (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,0,0,0,255);
                break;
            case 'e':
                break;
        }
        switch(color[num_ball + 1])
        {
            case 'b':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,640 - (100) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),400 - (100) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x))),35,0,0,0,255);
                break;
            case 'e':
                break;
        }
        }
        //SDL_RenderPresent(m_renderer);
        if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT && chekcker && !chekcker_key_pause)
        {
            Mix_PlayChannel(-1,enfejar,0);
            double dx1 = (x - center.x)/(sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
            double dy1 = (y - center.y)/(sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
            char cl = colors[rand()%4];
            Ball tmp;
            tmp.makan.x = 640 + (80) * ((x - center.x)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
            tmp.makan.y = 400 + (80) * ((y - center.y)/sqrt((y - center.y) * (y - center.y) + (x - center.x) * (x - center.x)));
            tmp.cl = color[num_ball];
            tmp.dx = dx1;
            tmp.dy = dy1;
            balls.push_back(tmp);
            chekcker = false;
            num_ball++;
            color.push_back(cl);
        }
        if(e->type = SDL_MOUSEBUTTONUP)
        {
            chekcker = true;
        }
        for(int i=0;i<num_ball && !chekcker_key_pause;i++)
        {
            if(balls[i].type == 0){
            switch(balls[i].cl)
            {
            case 'b':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,75,68,193,255);
                break;
            case 'r':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,232,23,31,255);
                break;
            case 'y':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,241,216,14,255);
                break;
            case 'g':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,72,222,18,255);
                break;
            case 'f':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,250,107,15,255);
                break;
            case 'm':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,250,15,207,255);
                break;
            case 't':
                filledCircleRGBA(m_renderer,balls[i].makan.x,balls[i].makan.y,35,0,0,0,255);
                break;
            case 'e':
                break;
            }
            }
            //SDL_RenderPresent(m_renderer);
            if(balls[i].vaziat)
            {
                balls[i].makan.x += balls[i].dx * v;
                balls[i].makan.y += balls[i].dy * v;
            }
            for(int j=0;j<num_ball_comming && num_ball_comming != 0;j++)
            {
                if(colapse(balls[i],balls_comming[j]))
                {
                    if(balls[i].cl == 'f')
                    {
                        balls[i].vaziat = false;
                    auto element1 = balls.begin() + i;
                    auto element2 = color.begin() + i;
                    balls.erase(element1);
                    color.erase(element2);
                    num_ball--;
                        fireb(j);
                        break;
                    }else if(balls[i].cl == 'm')
                    {
                        balls[i].vaziat = false;
                        auto element1 = balls.begin() + i;
                        auto element2 = color.begin() + i;
                        balls.erase(element1);
                        color.erase(element2);
                        num_ball--;
                        switch(balls_comming[j].cl)
                        {
                        case 'b':
                            multi_color('b');
                            break;
                        case 'g':
                            multi_color('g');
                            break;
                        case 'r':
                            multi_color('r');
                            break;
                        case 'y':
                            multi_color('y');
                            break;
                        }
                        break;
                    }
                    else if(balls[i].cl == 't')
                    {
                        balls[i].vaziat = false;
                        auto element1 = balls.begin() + i;
                        auto element2 = color.begin() + i;
                        balls.erase(element1);
                        color.erase(element2);
                        num_ball--;
                        bomb(j);
                        break;
                    }
                    else
                    {
                    balls_comming.insert(balls_comming.begin() + j,balls[i]);
                    balls[i].vaziat = false;
                    auto element1 = balls.begin() + i;
                    auto element2 = color.begin() + i;
                    balls.erase(element1);
                    color.erase(element2);
                    num_ball--;
                        if(bisharaf(j) >= 2)
                        {
                            fucking_game(j);
                        }
                        else{
                            num_ball_comming++;
                        }
                        break;
                    }

//                    while(bisharaf(j) >= 2)
//                    {
//                        fucking_game(j);
//                    }
                }
            }
            if(balls[i].makan.x  > W || balls[i].makan.x  < 0 || balls[i].makan.y  > H || balls[i].makan.y < 0)
            {
                auto element1 = balls.begin() + i;
                auto element2 = color.begin() + i;
                balls.erase(element1);
                color.erase(element2);
                num_ball--;
            }
        }
        for(int i=num_ball_comming;i>=0 && num_ball_comming != 0 ;i--)
        {
                switch(balls_comming[num_ball_comming- i].cl)
                {
                case 'b':
                    balls_comming[num_ball_comming- i].makan.x = masir2[i+cnt].x;
                    balls_comming[num_ball_comming- i].makan.y = masir2[i+cnt].y;
                    filledCircleRGBA(m_renderer,masir2[i+cnt].x,masir2[i+cnt].y,35,75,68,193,255);
                    break;
                case 'r':
                     balls_comming[num_ball_comming- i].makan.x = masir2[i+cnt].x;
                    balls_comming[num_ball_comming- i].makan.y = masir2[i+cnt].y;
                    filledCircleRGBA(m_renderer,masir2[i+cnt].x,masir2[i+cnt].y,35,232,23,31,255);
                    break;
                case 'y':
                    balls_comming[num_ball_comming- i].makan.x = masir2[i+cnt].x;
                    balls_comming[num_ball_comming- i].makan.y = masir2[i+cnt].y;
                    filledCircleRGBA(m_renderer,masir2[i+cnt].x,masir2[i+cnt].y,35,241,216,14,255);
                    break;
                case 'g':
                    balls_comming[num_ball_comming- i].makan.x = masir2[i+cnt].x;
                    balls_comming[num_ball_comming- i].makan.y = masir2[i+cnt].y;
                    filledCircleRGBA(m_renderer,masir2[i+cnt].x,masir2[i+cnt].y,35,72,222,18,255);
                    break;
                }
        }
        if(timer.getTicks() >= 1000 && timer2.getTicks()<=81000 && !chekcker_key_pause )
        {
            Ball tmp;
            char cl = colors[rand()%4];
            tmp.cl = cl;
            tmp.dx=0;
            tmp.dy = 0;
            tmp.makan.x = 0;
            tmp.makan.y = 0;
            tmp.vaziat = true;
            balls_comming.push_back(tmp);
            num_ball_comming++;
            num_ball_comming_cnst++;
            time--;
            timer.restart();
        }
        if(timer2.getTicks() >= 80000 && timer.getTicks()>=1000 && !chekcker_key_pause)
        {
            timer.restart();
            cnt++;
        }
        betterText(m_renderer,to_string(time),50,10,255,255,255,255,25,"@AlloFont(Airfool).otf");
        betterText(m_renderer,to_string(score),10,10,255,255,255,255,25,"@AlloFont(Airfool).otf");
        if(num_ball_comming>45 )
        {

            betterText(m_renderer,"Game Over!!!!!",300,400,232,23,31,255,70,"@AlloFont(Airfool).otf");
             SDL_RenderPresent(m_renderer);
            break;
        }
        if(num_ball_comming + cnt > 45)
        {
            betterText(m_renderer,"Game Over!!!!!",300,400,232,23,31,255,70,"@AlloFont(Airfool).otf");
             SDL_RenderPresent(m_renderer);
            break;
        }
        betterText(m_renderer,to_string(powers[0]),40,640,255,255,255,255,25,"@AlloFont(Airfool).otf");
        betterText(m_renderer,to_string(powers[2]),40,690,255,255,255,255,25,"@AlloFont(Airfool).otf");
        betterText(m_renderer,to_string(powers[1]),40,740,255,255,255,255,25,"@AlloFont(Airfool).otf");
        if(num_ball_comming <= 0 && num_ball_comming_cnst >= 80)
        {
            SDL_RenderClear(m_renderer);
            im2->render();
            betterText(m_renderer,"You won !!!!!",400,350,72,222,18,255,70,"@AlloFont(Airfool).otf");
            SDL_RenderPresent(m_renderer);
            break;
        }
        if(chekcker_key_pause)
        {
            pause->render();
        }
        SDL_RenderPresent(m_renderer);
    }
}
int main( int argc, char * argv[] )
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44000,MIX_DEFAULT_FORMAT,2,2048);
    background = Mix_LoadMUS("dash2.mp3");
    jumpeffect = Mix_LoadWAV("buttonsound.wav");
    enfejar = Mix_LoadWAV("enfejar.wav");
    Mix_PlayMusic(background,-1);
    string name[100];
masir[0].x= 1085;
masir[0].y= 112;
masir[1].x= 1010;
masir[1].y= 99;
masir[2].x= 934;
masir[2].y= 88;
masir[3].x= 860;
masir[3].y= 77;
masir[4].x= 786;
masir[4].y= 74;
masir[5].x= 710;
masir[5].y= 68;
masir[6].x= 634;
masir[6].y= 64;
masir[7].x= 558;
masir[7].y= 64;
masir[8].x= 482;
masir[8].y= 66;
masir[9].x= 409;
masir[9].y= 69;
masir[10].x= 333;
masir[10].y= 84;
masir[11].x= 262;
masir[11].y= 109;
masir[12].x= 198;
masir[12].y= 150;
masir[13].x= 143;
masir[13].y= 203;
masir[14].x= 98;
masir[14].y= 265;
masir[15].x= 72;
masir[15].y= 336;
masir[16].x= 62;
masir[16].y= 414;
masir[17].x= 68;
masir[17].y= 487;
masir[18].x= 88;
masir[18].y= 557;
masir[19].x= 127;
masir[19].y= 624;
masir[20].x= 181;
masir[20].y= 677;
masir[21].x= 248;
masir[21].y= 713;
masir[22].x= 317;
masir[22].y= 736;
masir[23].x= 392;
masir[23].y= 748;
masir[24].x= 467;
masir[24].y= 753;
masir[25].x= 543;
masir[25].y= 758;
masir[26].x= 618;
masir[26].y= 754;
masir[27].x= 692;
masir[27].y= 745;
masir[28].x= 766;
masir[28].y= 734;
masir[29].x= 840;
masir[29].y= 721;
masir[30].x= 914;
masir[30].y= 703;
masir[31].x= 986;
masir[31].y= 680;
masir[32].x= 1054;
masir[32].y= 650;
masir[33].x= 1118;
masir[33].y= 609;
masir[34].x= 1168;
masir[34].y= 550;
masir[35].x= 1198;
masir[35].y= 479;
masir[36].x= 1208;
masir[36].y= 403;
masir[37].x= 1199;
masir[37].y= 329;
masir[38].x= 1159;
masir[38].y= 270;
masir[39].x= 1089;
masir[39].y= 252;
masir[40].x= 1024;
masir[40].y= 287;
masir[41].x= 1002;
masir[41].y= 359;
masir[42].x= 1003;
masir[42].y= 435;
masir[43].x= 1005;
masir[43].y= 512;
masir[44].x= 955;
masir[44].y= 562;
masir[45].x= 885;
masir[45].y= 589;
masir[46].x= 814;
masir[46].y= 613;
masir[47].x= 739;
masir[47].y= 629;
masir[48].x= 662;
masir[48].y= 637;
masir[49].x= 588;
masir[49].y= 644;
masir[50].x= 514;
masir[50].y= 643;
masir[51].x= 439;
masir[51].y= 639;
masir[52].x= 363;
masir[52].y= 624;
masir[53].x= 290;
masir[53].y= 595;
masir[54].x= 235;
masir[54].y= 542;
masir[55].x= 208;
masir[55].y= 472;
masir[56].x= 203;
masir[56].y= 396;
masir[57].x= 221;
masir[57].y= 323;
masir[58].x= 264;
masir[58].y= 263;
masir[59].x= 327;
masir[59].y= 225;
masir[60].x= 398;
masir[60].y= 198;
masir[61].x= 474;
masir[61].y= 187;
masir[62].x= 549;
masir[62].y= 179;
masir[63].x= 625;
masir[63].y= 180;
masir[64].x= 701;
masir[64].y= 182;
masir[65].x= 777;
masir[65].y= 189;
masir[66].x= 867;
masir[66].y= 194;
masir[67].x= 1154;
masir[67].y= 125;
    masir2[0].x = 85 ;
    masir2[0].y = 643 ;
    masir2[1].x = 79 ;
    masir2[1].y = 568 ;
    masir2[2].x = 74 ;
    masir2[2].y = 492 ;
    masir2[3].x = 72 ;
    masir2[3].y = 415 ;
    masir2[4].x = 72 ;
    masir2[4].y = 338 ;
    masir2[5].x = 80 ;
    masir2[5].y = 262 ;
    masir2[6].x = 93 ;
    masir2[6].y = 186 ;
    masir2[7].x = 122 ;
    masir2[7].y = 117 ;
    masir2[8].x = 188 ;
    masir2[8].y = 74 ;
    masir2[9].x = 264 ;
    masir2[9].y = 65 ;
    masir2[10].x = 336 ;
    masir2[10].y = 88 ;
    masir2[11].x = 376 ;
    masir2[11].y = 152 ;
    masir2[12].x = 366 ;
    masir2[12].y = 227 ;
    masir2[13].x = 333 ;
    masir2[13].y = 295 ;
    masir2[14].x = 292 ;
    masir2[14].y = 360 ;
    masir2[15].x = 263 ;
    masir2[15].y = 432 ;
    masir2[16].x = 251 ;
    masir2[16].y = 506 ;
    masir2[17].x = 256 ;
    masir2[17].y = 583 ;
    masir2[18].x = 286 ;
    masir2[18].y = 653 ;
    masir2[19].x = 345 ;
    masir2[19].y = 700 ;
    masir2[20].x = 417 ;
    masir2[20].y = 724 ;
    masir2[21].x = 492 ;
    masir2[21].y = 735 ;
    masir2[22].x = 569 ;
    masir2[22].y = 734 ;
    masir2[23].x = 644 ;
    masir2[23].y = 735 ;
    masir2[24].x = 720 ;
    masir2[24].y = 735 ;
    masir2[25].x = 796 ;
    masir2[25].y = 734 ;
    masir2[26].x = 872 ;
    masir2[26].y = 733 ;
    masir2[27].x = 948 ;
    masir2[27].y = 728 ;
    masir2[28].x = 1025 ;
    masir2[28].y = 725 ;
    masir2[29].x = 1102 ;
    masir2[29].y = 716 ;
    masir2[30].x = 1169 ;
    masir2[30].y = 685 ;
    masir2[31].x = 1215 ;
    masir2[31].y = 626 ;
    masir2[32].x = 1232 ;
    masir2[32].y = 553 ;
    masir2[33].x = 1225 ;
    masir2[33].y = 477 ;
    masir2[34].x = 1198 ;
    masir2[34].y = 405 ;
    masir2[35].x = 1145 ;
    masir2[35].y = 350 ;
    masir2[36].x = 1077 ;
    masir2[36].y = 313 ;
    masir2[37].x = 1007 ;
    masir2[37].y = 283 ;
    masir2[38].x = 937 ;
    masir2[38].y = 255 ;
    masir2[39].x = 865 ;
    masir2[39].y = 229 ;
    masir2[40].x = 807 ;
    masir2[40].y = 181 ;
    masir2[41].x = 823 ;
    masir2[41].y = 113 ;
    masir2[42].x = 894 ;
    masir2[42].y = 82 ;
    masir2[43].x = 969 ;
    masir2[43].y = 78 ;
    masir2[44].x = 1044 ;
    masir2[44].y = 86 ;
    masir2[45].x = 1121 ;
    masir2[45].y = 94 ;
    masir2[46].x = 1199 ;
    masir2[46].y = 103 ;
    char color[4] = {'r','g','b','y'};
    int colors[68];
    //Mix_FreeMusic(background) //ghat krdn music

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    TTF_Init();
    SDL_Texture *texture, *text;
    TTF_Font* font;
    //Pass the focus to the drawing window
    SDL_RaiseWindow(m_window);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    W = DM.w;
    H = DM.h;
    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
    SDL_RenderClear( m_renderer );

    // Show the window
    SDL_RenderPresent( m_renderer );


    while(e->type != SDL_KEYDOWN)
        SDL_PollEvent(e);

    //Loading image
    //image bg = new image("pic/GameBG.png", 0, 0, 1.0, m_renderer);

    e->type = 0;
    int x,y;
    bool t = true;
    int flag = 0;
    while(t)
    {
        srand(time(NULL));
        SDL_PollEvent(e);
        SDL_ShowCursor(SDL_ENABLE);
        x=e->button.x;
        y=e->button.y;
        SDL_RenderClear(m_renderer);
        if(flag == 0)
        {
        backg->render();
        start_btn1->pic->render();
        setting_btn1->pic->render();
        leaderboard_btn1->pic->render();
        if(x>=545 && x<=725 && y>=365 && y<=429)
        {
            start_btn2->pic->render();
        }
        if(x>=545 && x<=725 && y>=525 && y<=598)
        {
            setting_btn2->pic->render();
        }
        if(x>=545 && x<=725 && y>=435 && y<=521)
        {
            leaderboard_btn2->pic->render();
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && start_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 1;
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && setting_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 2;
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && leaderboard_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = -1;
        }
        SDL_RenderPresent( m_renderer );
        e->type = 0;
        }
        if(flag == 1)
        {
        backg->render();
        login_btn1->pic->render();
        register_btn1->pic->render();
        back_btn1->pic->render();
        if(x>=545 && x<=725 && y>=395 && y<=459)
        {
            login_btn2->pic->render();
        }
        if(x>=545 && x<=725 && y>=495 && y<=568)
        {
            register_btn2->pic->render();
        }
         if(x>=100 && x<=228 && y>=600 && y<=664)
        {
            back_btn2->pic->render();
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && back_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 0;
        }
         if(e->type == SDL_MOUSEBUTTONDOWN && login_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 3;
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && register_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 4;
        }
        SDL_RenderPresent( m_renderer );
        e->type = 0;
        }
        if(flag == 2)
        {
        backg->render();
        music_btn1->pic->render();
        help_btn1->pic->render();
        back_btn1->pic->render();
        if(x>=545 && x<=725 && y>=395 && y<=459)
        {
            music_btn2->pic->render();
        }
        if(x>=545 && x<=725 && y>=465 && y<=551)
        {
            help_btn2->pic->render();
        }
        if(x>=122 && x<=250 && y>=600 && y<=664)
        {
            back_btn2->pic->render();
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && back_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 0;
        }
         if(e->type == SDL_MOUSEBUTTONDOWN && music_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 5;
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && help_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 6;
        }
        SDL_RenderPresent( m_renderer );
        e->type = 0;
        }
        if(flag == -1)
        {
        backg->render();
        back_btn1->pic->render();
        if(x>=122 && x<=250 && y>=600 && y<=664)
        {
            back_btn2->pic->render();
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && back_btn1->btn_clicked(x,y))
        {
            Mix_PlayChannel(-1,jumpeffect,0);
            flag = 0;
        }
        SDL_RenderPresent( m_renderer );
        e->type = 0;
        }
        if(flag == 3)
        {
        SDL_RenderClear(m_renderer);
        backg->render();
        username_btn->pic->render();
        password_btn->pic->render();
        save_btn1->pic->render();
        back_btn1->pic->render();
        string input="",username,password;
        int mode  = 0;
        font = TTF_OpenFont("@AlloFont(Airfool).otf", 50);
        SDL_StartTextInput();
        while(mode <= 2){
        SDL_RenderClear(m_renderer);
        backg->render();
        username_btn->pic->render();
        password_btn->pic->render();
        save_btn1->pic->render();
        back_btn1->pic->render();
        if(mode == 1){
            betterText(m_renderer,username,550,340,130,156,145,255,50,"@AlloFont(Airfool).otf");
        }
                static const unsigned char* keys = SDL_GetKeyboardState( NULL );
                SDL_Rect dest;
                while ( SDL_PollEvent( e ) != 0 && (e->type == SDL_TEXTINPUT || SDL_KEYDOWN)) {
                    switch (e->type) {
                    case SDL_TEXTINPUT:
                        if(mode == 0)
                            input += e->text.text;
                        else
                            input += '*';
                        break;
                    case SDL_KEYDOWN:
                        if (e->key.keysym.sym == SDLK_BACKSPACE && input.size()) {
                            input.pop_back();
                        }else if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_KP_ENTER)
                        {
                                mode++;
                                if(mode == 1)
                                    username = input;
                                else
                                    password = input;
                                input = "";
                        }
                        break;
                    }
                }
                switch(e->type){
                    case SDL_MOUSEMOTION:
                        x=e->button.x;
                        y=e->button.y;
                        if(x>=100 && x<=353 && y>=600 && y<=703)
                        {
                            back_btn2->pic->render();
                        }
                        if(x>=800 && x<=1160 && y>=600 && y<=703)
                        {
                            save_btn2->pic->render();
                        }
                        SDL_RenderPresent(m_renderer);
                    case SDL_MOUSEBUTTONDOWN:
                        x=e->button.x;
                        y=e->button.y;
                        if(e->type == SDL_MOUSEBUTTONDOWN && back_btn1->btn_clicked(x,y))
                        {
                            Mix_PlayChannel(-1,jumpeffect,0);
                            flag = 1;
                        }
                        if(e->type == SDL_MOUSEBUTTONDOWN && save_btn1->btn_clicked(x,y))
                        {
                            Mix_PlayChannel(-1,jumpeffect,0);
                            flag = 7;
                            break;
                        }
                    }
                SDL_Color foreground = { 130,156,145 };
                if ( input.size() && mode == 0) {
                    SDL_Surface* text_surf = TTF_RenderText_Solid(font, input.c_str(), foreground);
                    text = SDL_CreateTextureFromSurface(m_renderer, text_surf);
                    dest.x = 550 ;
                    dest.y = 340;
                    dest.w = text_surf->w;
                    dest.h = text_surf->h;
                    SDL_RenderCopy(m_renderer, text, NULL, &dest);
                    name[mode] = input;
                    SDL_DestroyTexture(text);
                    SDL_FreeSurface(text_surf);
                }
                else if ( input.size() && mode == 1) {
                    SDL_Surface* text_surf = TTF_RenderText_Solid(font, input.c_str(), foreground);
                    text = SDL_CreateTextureFromSurface(m_renderer, text_surf);
                    dest.x = 550 ;
                    dest.y = 440;
                    dest.w = text_surf->w;
                    dest.h = text_surf->h;
                    SDL_RenderCopy(m_renderer, text, NULL, &dest);
                    SDL_DestroyTexture(text);
                    SDL_FreeSurface(text_surf);
                }
                SDL_RenderPresent(m_renderer);
                if(flag == 1)
                    break;
            }
            int check;
            ifstream f;
            f.open("user.txt", ios_base::in);
            while(! f.eof())
            {
                string s;
                getline(f, s, '\n');
                if (s.compare(username) == 0)
                {
                    getline(f, s, '\n');
                    if (s.compare(password) == 0)
                    {
                        check = 1;
                        break;
                    }
                }
            }
            if(check == 1)
            {
                cout<<"Welcome"<<endl;
                flag = 7;
            }else
            {
                cout<<"your password or username is wrong :("<<endl;
                flag = 0;
            }
            SDL_StopTextInput();
            TTF_CloseFont( font );
            SDL_RenderPresent( m_renderer );
            e->type = 0;
        }

        if(flag == 4)
        {
        SDL_RenderClear(m_renderer);
        backg->render();
        username_btn->pic->render();
        password_btn->pic->render();
        save_btn1->pic->render();
        back_btn1->pic->render();
        string input="",username,password;
        int mode  = 0;
        font = TTF_OpenFont("@AlloFont(Airfool).otf", 50);
        SDL_StartTextInput();
        while(mode < 2){
        SDL_RenderClear(m_renderer);
        backg->render();
        username_btn->pic->render();
        password_btn->pic->render();
        save_btn1->pic->render();
        back_btn1->pic->render();
        if(mode == 1){
            betterText(m_renderer,username,550,340,130,156,145,255,50,"@AlloFont(Airfool).otf");
        }
                static const unsigned char* keys = SDL_GetKeyboardState( NULL );
                SDL_Rect dest;
                while ( SDL_PollEvent( e ) != 0 && (e->type == SDL_TEXTINPUT || SDL_KEYDOWN)) {
                    switch (e->type) {
                    case SDL_TEXTINPUT:
                        input += e->text.text;
                        break;
                    case SDL_KEYDOWN:
                        if (e->key.keysym.sym == SDLK_BACKSPACE && input.size()) {
                            input.pop_back();
                        }else if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_KP_ENTER)
                        {
                                mode++;
                                if(mode == 1)
                                    username = input;
                                else
                                    password = input;
                                input = "";
                        }
                        break;
                    }
                }
                switch(e->type){
                    case SDL_MOUSEMOTION:
                        x=e->button.x;
                        y=e->button.y;
                        if(x>=100 && x<=353 && y>=600 && y<=703)
                        {
                            back_btn2->pic->render();
                        }
                        if(x>=800 && x<=1160 && y>=600 && y<=703)
                        {
                            save_btn2->pic->render();
                        }
                        SDL_RenderPresent(m_renderer);
                    case SDL_MOUSEBUTTONDOWN:
                        x=e->button.x;
                        y=e->button.y;
                        if(e->type == SDL_MOUSEBUTTONDOWN && back_btn1->btn_clicked(x,y))
                        {
                            Mix_PlayChannel(-1,jumpeffect,0);
                            flag = 1;
                        }
                        if(e->type == SDL_MOUSEBUTTONDOWN && save_btn1->btn_clicked(x,y))
                        {
                            Mix_PlayChannel(-1,jumpeffect,0);
                            flag = 7;
                        }
                    }
                SDL_Color foreground = { 130,156,145 };
                if ( input.size() && mode == 0) {
                    SDL_Surface* text_surf = TTF_RenderText_Solid(font, input.c_str(), foreground);
                    text = SDL_CreateTextureFromSurface(m_renderer, text_surf);
                    dest.x = 550 ;
                    dest.y = 340;
                    dest.w = text_surf->w;
                    dest.h = text_surf->h;
                    SDL_RenderCopy(m_renderer, text, NULL, &dest);
                    name[mode] = input;
                    SDL_DestroyTexture(text);
                    SDL_FreeSurface(text_surf);
                }
                else if ( input.size() && mode == 1) {
                    SDL_Surface* text_surf = TTF_RenderText_Solid(font, input.c_str(), foreground);
                    text = SDL_CreateTextureFromSurface(m_renderer, text_surf);
                    dest.x = 550 ;
                    dest.y = 440;
                    dest.w = text_surf->w;
                    dest.h = text_surf->h;
                    SDL_RenderCopy(m_renderer, text, NULL, &dest);
                    SDL_DestroyTexture(text);
                    SDL_FreeSurface(text_surf);
                }
                SDL_RenderPresent(m_renderer);
                if(flag == 1)
                    break;
            }
            int check;
            ofstream f;
            f.open("user.txt",ios::app);
            f<<username<<endl<<password<<endl;
            f.close();
            SDL_StopTextInput();
            TTF_CloseFont( font );
            flag = 7;
            SDL_RenderPresent( m_renderer );
            e->type = 0;
        }
        if(flag == 5)
        {
            backg->render();
            music1_btn1->pic->render();
            music2_btn1->pic->render();
            music3_btn1->pic->render();
            mute_btn1->pic->render();
            back_btn1->pic->render();
            if(x>=122 && x<=250 && y>=600 && y<=664)
            {
                back_btn2->pic->render();
            }
            if(x>=545 && x<=725 && y>=365 && y<=429)
            {
                music1_btn2->pic->render();
            }
            if(x>=545 && x<=725 && y>=445 && y<=509)
            {
                music2_btn2->pic->render();
            }
            if(x>=545 && x<=725 && y>=525 && y<=598)
            {
                music3_btn2->pic->render();
            }
            if(x>=545 && x<=725 && y>=605 && y<=669)
            {
                mute_btn2->pic->render();
            }
            if(e->type == SDL_MOUSEBUTTONDOWN && back_btn1->btn_clicked(x,y))
            {
                flag = 0;
                SDL_Delay(100);
            }
            if(e->type == SDL_MOUSEBUTTONDOWN && music1_btn1->btn_clicked(x,y))
            {
                Mix_FreeMusic(background);
                background = Mix_LoadMUS("dash2.mp3");
                Mix_PlayMusic(background,-1);
            }
            if(e->type == SDL_MOUSEBUTTONDOWN && music2_btn1->btn_clicked(x,y))
            {
                Mix_FreeMusic(background);
                background = Mix_LoadMUS("dash3.mp3");
                Mix_PlayMusic(background,-1);
            }
            if(e->type == SDL_MOUSEBUTTONDOWN && music3_btn1->btn_clicked(x,y))
            {
                Mix_FreeMusic(background);
                background = Mix_LoadMUS("dash.mp3");
                Mix_PlayMusic(background,-1);
            }
             if(e->type == SDL_MOUSEBUTTONDOWN && mute_btn1->btn_clicked(x,y))
            {
                Mix_FreeMusic(background);
                Mix_FreeChunk(jumpeffect);
            }
            SDL_RenderPresent( m_renderer );
            e->type = 0;
        }
        if(flag == 6)
        {
            backg->render();
            help->render();
        back_btn1->pic->render();
        if(x>=122 && x<=250 && y>=600 && y<=664)
        {
            back_btn2->pic->render();
        }
        if(e->type == SDL_MOUSEBUTTONDOWN && back_btn1->btn_clicked(x,y))
        {
            flag = 2;
            SDL_Delay(100);
        }
            SDL_RenderPresent(m_renderer);
        }
        if(flag == 7)
        {
            backg->render();
            back_btn1->pic->render();
            map1_btn1->pic->render();
            map2_btn1->pic->render();
            if(x>=122 && x<=250 && y>=600 && y<=664)
            {
                back_btn2->pic->render();
            }
            if(x>=545 && x<=725 && y>=365 && y<=429)
            {
                map1_btn2->pic->render();
            }
            if(x>=545 && x<=725 && y>=525 && y<=598)
            {
                map2_btn2->pic->render();
            }
            if(e->type == SDL_MOUSEBUTTONDOWN && back_btn1->btn_clicked(x,y))
            {
            flag = 1;
            SDL_Delay(100);
            }
            if(e->type == SDL_MOUSEBUTTONDOWN && map1_btn1->btn_clicked(x,y))
            {
                flag = 8;
            }
            if(e->type == SDL_MOUSEBUTTONDOWN && map2_btn1->btn_clicked(x,y))
            {
                flag = 9;
            }
            SDL_RenderPresent( m_renderer );
            e->type = 0;
        }
        if(flag == 8)
        {
            game();
            break;
        }
        if(flag == 9)
        {
            game2();
            break;
        }
   }
    while(e->type != SDL_KEYDOWN)
        SDL_PollEvent(e);

    //Finalize and free resources
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    IMG_Quit();
    SDL_Quit();
    return 0;

}
