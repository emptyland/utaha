#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>

int texture_demo(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    return texture_demo(argc, argv);
}


int texture_demo(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *main_window = SDL_CreateWindow("texture", SDL_WINDOWPOS_UNDEFINED,
                                               SDL_WINDOWPOS_UNDEFINED, 288 * 2, 512,
                                               SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    SDL_Surface *screen = SDL_GetWindowSurface(main_window);

    IMG_Init(IMG_INIT_PNG);
    SDL_Surface *img_surface = IMG_Load("assets/test-player.png");
    SDL_Surface *optimized_surface = SDL_ConvertSurface(img_surface, screen->format, NULL);


    TTF_Font *font = TTF_OpenFont("assets/fonts/default.ttf", 28);
    SDL_Surface* text_surface = TTF_RenderUTF8_Solid(font, u8"Hello, Asshole! 干干干", {0xff, 0xff, 0xff});

    SDL_Renderer *renderer = SDL_GetRenderer(main_window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, optimized_surface);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);


    bool quit = false;
    SDL_Event e;
    while( !quit ) {
        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0) {
            //User requests quit
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }

        //Clear screen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_Rect rect = {.x=0, .y=0, .w=text_surface->w, .h=text_surface->h};
        SDL_RenderCopy(renderer, text_texture, nullptr, &rect);
        SDL_RenderPresent(renderer);
    }

    IMG_SavePNG(text_surface, "assets/saved-text.png");

    //Deallocate surface
    SDL_FreeSurface(screen);
    //Destroy window
    SDL_DestroyWindow(main_window);
    //Quit SDL subsystems
    SDL_Quit();
    return 0;
}
