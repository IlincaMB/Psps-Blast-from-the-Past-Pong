#include <iostream>
#include <time.h>
#include <conio.h>
#include <SDL.h>

// define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//define SDL Window related variables
SDL_Window* window = NULL;
SDL_Renderer* windowRenderer = NULL;
SDL_Event currentEvent;

enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };
bool initWindow() {

    bool success = true;

    //Try to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    }
    else {

        //Try to create the window
        window = SDL_CreateWindow(
            "SDL Hello Window Example",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if (window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        }
        else {

            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if (windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            }
            else {

                //Set background color
                SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);

                //Apply background color
                SDL_RenderClear(windowRenderer);
            }
        }
    }

    return success;
}
void drawFrame() {

    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);
    SDL_RenderClear(windowRenderer);

    SDL_RenderPresent(windowRenderer);
}
class ball
{
private:
    int x, y;
    int original_X, original_Y;
    eDir dir;
public:
    ball(int posX, int posY)
    {
        original_X = posX;
        original_Y = posY;
        x = posX; y = posY;
        dir = STOP;
    }
    void Reset()
    {
        x = original_X; y = original_Y;
        dir = STOP;
    }
    void changeDirection(eDir direction)
    {
        dir = direction;
    }
    void randomDirection()
    {
        dir = (eDir)((rand() % 6) + 1);
    }
    inline int getX() { return x; }
    inline int getY() { return y; }
    inline eDir getDirection() { return dir; }
    void Move()
    {
        switch (dir)
        {
        case STOP:
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UPLEFT:
            x--; y--;
            break;
        case DOWNLEFT:
            x--; y++;
            break;
        case UPRIGHT:
            x++; y--;
            break;
        case DOWNRIGHT:
            x++; y++;
            break;
        default:
            break;
        }
    }
};
class paleta
{
private:
    int x, y;
    int original_X, original_Y;
public:
    paleta()
    {
        x = y = 0;
    }
    paleta(int posX, int posY) : paleta()
    {
        original_X = posX;
        original_Y = posY;
        x = posX;
        y = posY;
    }
    inline void Reset() { x = original_X; y = original_Y; }
    inline int getX() { return x; }
    inline int getY() { return y; }
    inline void moveUp() { y--; }
    inline void moveDown() { y++; }
};
class game
{
private:
    int width, height;
    int up, down;
    bool quit;
    ball* ball;
    paleta* jucator;
public:
    game()
    {
        srand(time(NULL));
        quit = false;
        up = SDLK_UP; 
        down = SDLK_DOWN;
        ball = new ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
       jucator = new paleta(1, WINDOW_HEIGHT / 2 - 3);
    }
    ~game()
    {
        delete ball, jucator;
    }
    void Input()
    {
        ball->Move();

        int ballx = ball->getX();
        int bally = ball->getY();
        int jucatorx = jucator->getX();
        int jucatory = jucator->getY();

        if (_kbhit())
        {
            char current = _getch();
            if (current == up)
                if (jucatory > 0)
                    jucator->moveUp();
            if (current == down)
                if (jucatory + 4 < height)
                    jucator->moveDown();

            if (ball->getDirection() == STOP)
                ball->randomDirection();

            if (current == 'q')
                quit = true;
        }
    }
    void Logic()
    {
        int ballx = ball->getX();
        int bally = ball->getY();
        int jucatorx = jucator->getX();
        int jucatory = jucator->getY();

        //left paddle
        for (int i = 0; i < 4; i++)
            if (ballx == jucatorx + 1)
                if (bally == jucatory + i)
                    ball->changeDirection((eDir)((rand() % 3) + 4));

        //bottom wall
        if (bally == height - 1)
            ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
        //top wall
        if (bally == 0)
            ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
        //right wall
    }
    void Run()
    {
        while (!quit)
        {
            drawFrame();
            Input();
            Logic();
            drawFrame();
        }
    }
};
void cleanup() {

    //Destroy renderer
    if (windowRenderer) {

        SDL_DestroyRenderer(windowRenderer);
        windowRenderer = NULL;
    }

    //Destroy window
    if (window) {

        SDL_DestroyWindow(window);
        window = NULL;
    }

    //Quit SDL
    SDL_Quit();
}
int main()
{
    if (!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }
    game();
    cleanup();
    return 0;
}