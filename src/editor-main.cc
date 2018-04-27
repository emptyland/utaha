#include "sample-render-entity.h"
#include "root-render-entity.h"
#include "ui-flat-button.h"
#include "ui-flat-input-box.h"
#include "ui-layout.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-menu.h"
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

    virtual int OnCommand(utaha::UIComponent *sender, int cmd_id, void *param, bool *is_break) override {
        return 0;
    }
};

int texture_demo(int argc, char *argv[]);
int render_demo(int argc, char *argv[]);
int layout_demo(int argc, char *argv[]);
int main_menu_demo(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    //return texture_demo(argc, argv);
    //return render_demo(argc, argv);
    //return layout_demo(argc, argv);
    return main_menu_demo(argc, argv);
}

int main_menu_demo(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *main_window = SDL_CreateWindow("render", SDL_WINDOWPOS_UNDEFINED,
                                               SDL_WINDOWPOS_UNDEFINED, 288 * 2, 512,
                                               SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    TTF_Font *font = TTF_OpenFont("assets/fonts/default.ttf", 20);
    utaha::RootRenderEntity root;

    auto layout = new utaha::UILayout(main_window);
    layout->set_debug_mode(true);
    layout->set_padding_size(5);
    layout->set_vertical_alignment(utaha::UILayout::ALIGN_LEFT_OR_TOP);
    layout->set_horizontal_aligment(utaha::UILayout::ALIGN_LEFT_OR_TOP);
    auto row = layout->AddRow(utaha::UILayout::ALIGN_LEFT_OR_TOP);

    auto menu = new utaha::UIFlatMenu(font);
    menu->set_bg_color({0x80, 0x8b, 0xff});
    menu->set_hot_color({0x00, 0x7b, 0xff});
    menu->set_font_color({0xff, 0xff, 0xff});
    menu->set_border_color({0xff, 0xff, 0xff});
    menu->AddItem("Open", 1, nullptr);
    menu->AddDiv();
    menu->AddItem("Save", 2, nullptr);
    menu->AddItem("Save as ...", 3, nullptr);

    auto main_menu = new utaha::UIFlatMenuGroup(font);
    main_menu->set_bg_color({0, 0, 0});
    main_menu->set_border_color({0xff, 0xff, 0xff});
    main_menu->set_hot_color({0x00, 0x7b, 0xff});
    main_menu->set_font_color({0xff, 0xff, 0xff});
    main_menu->set_h_padding_size(10);
    main_menu->set_v_padding_size(2);
    main_menu->AddColumn("File", 1, menu);
    main_menu->AddColumn("2", 2, nullptr);
    main_menu->AddColumn("3", 3, nullptr);
    main_menu->AddColumn("44", 4, nullptr);
    main_menu->AddColumn("Help", 5, nullptr);
    row->AddComponent(main_menu);

    root.InsertLL(layout);

    bool quit = false;
    bool is_break = false;
    SDL_Event e;
    while(!quit) {
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0) {
            //User requests quit
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            layout->OnEvent(&e, &is_break);
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

int layout_demo(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *main_window = SDL_CreateWindow("render", SDL_WINDOWPOS_UNDEFINED,
                                               SDL_WINDOWPOS_UNDEFINED, 288 * 2, 512,
                                               SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    TTF_Font *font = TTF_OpenFont("assets/fonts/default.ttf", 30);
    utaha::RootRenderEntity root;

    auto layout = new utaha::UILayout(main_window);
    layout->set_debug_mode(true);
    layout->set_padding_size(5);
    layout->set_vertical_alignment(utaha::UILayout::ALIGN_LEFT_OR_TOP);
    layout->set_horizontal_aligment(utaha::UILayout::ALIGN_CENTER);
    auto row = layout->AddRow(utaha::UILayout::ALIGN_LEFT_OR_TOP);

    auto btn1 = new utaha::UIFlatButton();
    btn1->set_id(1);
    btn1->set_name("demo");
    btn1->set_normal_color({0x00, 0x7b, 0xff});
    btn1->set_pressed_color({0x80, 0x8b, 0xff});
    btn1->mutable_rect()->w = 80;
    btn1->mutable_rect()->h = 40;
    btn1->SetNormalText("OK", {0xff, 0xff, 0xff}, font, renderer);
    btn1->SetPressedText("- -", {0, 0, 0}, font, renderer);
    row->AddComponent(btn1);

    btn1 = new utaha::UIFlatButton();
    btn1->set_id(2);
    btn1->set_name("demo");
    btn1->set_normal_color({0x00, 0x7b, 0xff});
    btn1->set_pressed_color({0x80, 0x8b, 0xff});
    btn1->mutable_rect()->w = 80;
    btn1->mutable_rect()->h = 40;
    btn1->SetNormalText("NO", {0xff, 0xff, 0xff}, font, renderer);
    btn1->SetPressedText("- -", {0, 0, 0}, font, renderer);
    row->AddComponent(btn1);

    btn1 = new utaha::UIFlatButton();
    btn1->set_id(3);
    btn1->set_name("demo");
    btn1->set_normal_color({0x00, 0x7b, 0xff});
    btn1->set_pressed_color({0x80, 0x8b, 0xff});
    btn1->mutable_rect()->w = 80;
    btn1->mutable_rect()->h = 40;
    btn1->SetNormalText("DEMO", {0xff, 0xff, 0xff}, font, renderer);
    btn1->SetPressedText("- -", {0, 0, 0}, font, renderer);
    row->AddComponent(btn1);

    auto inb1 = new utaha::UIFlatInputBox(font);
    inb1->mutable_rect()->w = 180;
    inb1->mutable_rect()->h = 48;
    inb1->set_id(2);
    inb1->set_name("demo");
    inb1->set_bg_color({0x80, 0x8b, 0xff});
    inb1->set_text_color({0xff, 0xff, 0xff});
    inb1->set_border_color({0xff, 0xff, 0xff});
    row = layout->AddRow(utaha::UILayout::ALIGN_CENTER);
    row->AddComponent(inb1);

    auto menu = new utaha::UIFlatMenu(font);
    menu->set_bg_color({0x80, 0x8b, 0xff});
    menu->set_hot_color({0x00, 0x7b, 0xff});
    menu->set_font_color({0xff, 0xff, 0xff});
    menu->set_border_color({0xff, 0xff, 0xff});
    menu->AddItem("Open", 1, nullptr);
    menu->AddDiv();
    menu->AddItem("Save", 2, nullptr);
    menu->AddItem("Save as ...", 3, nullptr);

    root.InsertLL(layout);
    root.InsertRR(menu);

    bool quit = false;
    bool is_break = false;
    SDL_Event e;
    while(!quit) {
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0) {
            //User requests quit
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            layout->OnEvent(&e, &is_break);
            menu->OnEvent(&e, &is_break);

            if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == 3) {
                    menu->Popup(e.button.x, e.button.y, renderer);
                }
            }
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

    auto inb1 = new utaha::UIFlatInputBox(font);
    inb1->set_rect({.x = 20, .y = 100, .w = 180, .h = 48});
    inb1->set_id(2);
    inb1->set_name("demo");
    inb1->set_bg_color({0x80, 0x8b, 0xff});
    inb1->set_text_color({0xff, 0xff, 0xff});
    inb1->set_border_color({0xff, 0xff, 0xff});
    inb1->AddListenner(listenner);

    //t1->InsertLL(t3);
    root.InsertLL(t1);
    root.InsertRL(t2);
    root.InsertRL(t3);
    root.InsertRL(btn1);
    root.InsertRL(inb1);

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
            inb1->OnEvent(&e, &is_break);
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
