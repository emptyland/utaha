#include "sample-render-entity.h"
#include "root-render-entity.h"
#include "ui-flat-button.h"
#include "interactive-listenner.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>

class DemoLayoutListenner : public utaha::InteractiveListenner {
public:
    virtual int OnClick(utaha::UIComponent *sender, bool */*is_break*/) override {
        printf("on click: %p\n", sender);
        return 0;
    }

    virtual int OnDoubleClick(utaha::UIComponent *sender, bool *is_break) override {
        return 0;
    }

    virtual int OnMouseMove(utaha::UIComponent *sender, int x, int y, bool *is_break) override {
        return 0;
    }
};

int texture_demo(int argc, char *argv[]);
int render_demo(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    //return texture_demo(argc, argv);
    return render_demo(argc, argv);
}


int render_demo(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *main_window = SDL_CreateWindow("render", SDL_WINDOWPOS_UNDEFINED,
                                               SDL_WINDOWPOS_UNDEFINED, 288 * 2, 512,
                                               SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    TTF_Font *font = TTF_OpenFont("assets/fonts/default.ttf", 30);
    utaha::RootRenderEntity root;
    utaha::SampleTextLabel *t1 = new utaha::SampleTextLabel(font);
    t1->SetTextShaded(u8"t1", {0xff, 0xff, 0xff}, {0x80, 0x80, 0x80}, renderer);
    t1->set_rect({.x=0, .y=0, .w=t1->surface()->w, .h=t1->surface()->h});

    utaha::SampleTextLabel *t2 = new utaha::SampleTextLabel(font);
    t2->SetTextSolid(u8"t2", {0xff, 0, 0xff}, renderer);
    t2->set_rect({.x=t2->surface()->w, .y=0, .w=t2->surface()->w, .h=t2->surface()->h});

    utaha::SampleTextLabel *t3 = new utaha::SampleTextLabel(font);
    t3->SetTextBlended("t3", {0, 0xff, 0xff}, renderer);
    t3->set_rect({.x=t2->rect().x + t2->rect().w, .y=0, .w=t3->surface()->w, .h=t3->surface()->h});

    auto listenner = new DemoLayoutListenner();

    auto btn1 = new utaha::UIFlatButton();
    btn1->set_normal_color({0x00, 0x7b, 0xff});
    btn1->set_pressed_color({0x80, 0x8b, 0xff});
    btn1->set_rect({.x = 20, .y = 50, .w = 80, .h = 40});
    btn1->set_id(1);
    btn1->set_name("demo");
    btn1->SetNormalText("OK", {0xff, 0xff, 0xff}, font, renderer);
    btn1->SetPressedText("- -", {0, 0, 0}, font, renderer);
    btn1->AddListenner(listenner);

    //t1->InsertLL(t3);
    root.InsertLL(t1);
    root.InsertRL(t2);
    root.InsertRL(t3);
    root.InsertRL(btn1);

    bool quit = false;
    bool is_break = false;
    SDL_Event e;
    while( !quit ) {
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0) {
            //User requests quit
            if(e.type == SDL_QUIT) {
                quit = true;
            }

            btn1->OnEvent(&e, &is_break);
        }
        //Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);
        root.OnRender(renderer);
        SDL_RenderPresent(renderer);
    }

    //Destroy window
    SDL_DestroyWindow(main_window);
    //Quit SDL subsystems
    SDL_Quit();
    return 0;
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
