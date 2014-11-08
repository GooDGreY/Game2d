#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "Engine.hpp"
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

bool loadMedia();

void close();


SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

Texture gFooTexture;
Texture gBackgroundTexture;

Player player;

bool init() {

    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        std::cout<<"SDL could not initialize! SDL Error: %s\n"<< SDL_GetError();
        success = false;
    } else {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
            std::cout<<"Warning: Linear texture filtering not enabled!";
        }

        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            std::cout<<"Window could not be created! SDL Error: %s\n"<< SDL_GetError();
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL ) {
                std::cout<<"Renderer could not be created! SDL Error: %s\n"<< SDL_GetError() ;
                success = false;
            } else {
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                    std::cout<<"SDL_image could not initialize! SDL_image Error: %s\n"<< IMG_GetError();
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {

    bool success = true;
    std::string resFolder = "../../res/";
    gFooTexture.Init(gRenderer);
    if( !gFooTexture.loadFromFile(resFolder+"foo.png" ) ) {
        std::cout<<"Failed to load Foo' texture image!\n";
        success = false;
    }

    player.Init(&gFooTexture);

    gBackgroundTexture.Init(gRenderer);
    if( !gBackgroundTexture.loadFromFile(resFolder+"background.png" ) ) {
        std::cout<<"Failed to load background texture image!\n";
        success = false;
    }

    return success;
}

void close() {
    gFooTexture.free();
    gBackgroundTexture.free();
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    IMG_Quit();
    SDL_Quit();
}

float _deltaTime;


Uint32 currentTime =  SDL_GetTicks();

void UpdateTime() {
    auto nowTime =  SDL_GetTicks();

    _deltaTime = nowTime - currentTime;
    currentTime = nowTime;

}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
#else
int main(int argc, char* argv[])
#endif
{
    if( !init() ) {
        std::cout<<"Failed to initialize!\n" ;
    } else {
        if( !loadMedia() ) {
            std::cout<< "Failed to load media!\n";
        } else {
            bool quit = false;
            SDL_Event event;

            player.SetPosition(240, 190);
            while( !quit ) {

                UpdateTime();
                while( SDL_PollEvent( &event ) != 0 ) {

                    switch(event.type) {
                    case SDL_QUIT : {
                        quit = true;
                        break;
                    }
                    case SDL_KEYDOWN : {
                        std::cout<<"Key press"<<event.key.keysym.sym<<std::endl;
                        switch(event.key.keysym.sym) {
                        case SDLK_LEFT: {
                            player.ToLeft(_deltaTime*1.2);
                            break;
                        }
                        case SDLK_RIGHT: {
                            player.ToRight(_deltaTime*1.2);
                            break;
                        }
                        case SDLK_UP: {
                            player.Jump(200);
                            break;
                        }
                        }
                    }
                    }

                }
                // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                // SDL_RenderClear( gRenderer );

                gBackgroundTexture.render( 0, 0 );
                player.Update(_deltaTime);
                player.Draw();
                //   gFooTexture.render( 240, 190 );
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();
    return 0;
}