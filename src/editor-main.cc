#include "sample-render-entity.h"
#include "root-render-entity.h"
#include "ui-flat-button.h"
#include "ui-flat-input-box.h"
#include "ui-layout.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-menu.h"
#include "ui-pic-grid-selector.h"
#include "ui-component-builder.h"
#include "ui-component-factory.h"
#include "interactive-listenner.h"
#include "ui-style-collection.h"
#include "ui-form.h"
#include "lua-utils.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>

namespace utaha {
    extern UIForm *CreateEditorForm();
}

//int style_demo(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    std::unique_ptr<utaha::UIForm> form(utaha::CreateEditorForm());
    if (!form->CreateWindow("Utaha-Editor", 800, 600)) {
        return -1;
    }
    return form->Run();
    //return style_demo(argc, argv);
}

//int style_demo(int argc, char *argv[]) {
//    SDL_Init(SDL_INIT_VIDEO);
//    TTF_Init();
//    SDL_Window *main_window = SDL_CreateWindow("render", SDL_WINDOWPOS_UNDEFINED,
//                                               SDL_WINDOWPOS_UNDEFINED, 288 * 2, 512,
//                                               SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
//    SDL_Renderer *renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
//    utaha::RootRenderEntity root;
//
//    utaha::UIStyleCollection styles;
//    auto err = styles.LoadFromFile("tests/001-styles-demo1.lua");
//    if (err) {
//        printf("%s\n", err);
//        return 1;
//    }
//    auto L = utaha::LuaUtils::GeneralOpenLua();
//    utaha::UIComponentBuilder::BindTo(L);
//
//    std::unique_ptr<utaha::UIComponentFactory> factory(utaha::CreateUIComponentStyleFactory(&styles));
//    auto builder = new utaha::UIComponentBuilder(factory.get(), main_window);
//
//    luabridge::getGlobalNamespace(L)
//    .beginNamespace(utaha::kLuaNamespace)
//        .addVariable("uiBuilder", &builder, false)
//    .endNamespace();
//
//    err = utaha::LuaUtils::ProtectedDoFile(L, "tests/005-layout-builder-demo1.lua");
//    if (err) {
//        printf("%s\n", err);
//        return 1;
//    }
//
//    utaha::UILayout *layout = luabridge::Stack<utaha::UILayout *>::get(L, lua_gettop(L));
//    lua_close(L);
//    delete builder;
//    builder = nullptr;
//
//    std::unique_ptr<utaha::UIPicGridSelector> selector(factory->CreatePicGridSelector("test.pic"));
//    selector->mutable_rect()->x = 0;
//    selector->mutable_rect()->y = layout->rect().h;
//    selector->mutable_rect()->w = 480;
//    selector->mutable_rect()->h = 256;
//    selector->set_grid_size_w(24);
//    selector->set_grid_size_h(32);
//    selector->LoadPicFromFile("assets/raw-02.png");
//
//    root.InsertLL(layout);
//    root.InsertLL(selector.get());
//
//    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//    bool quit = false;
//    bool is_break = false;
//    SDL_Event e;
//    while(!quit) {
//        //Handle events on queue
//        while(SDL_PollEvent(&e) != 0) {
//            //User requests quit
//            if(e.type == SDL_QUIT) {
//                quit = true;
//            }
//            selector->OnEvent(&e, &is_break);
//            layout->OnEvent(&e, &is_break);
//        }
//        //Clear screen
//        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
//        SDL_RenderClear(renderer);
//        root.OnRender(renderer);
//        SDL_RenderPresent(renderer);
//    }
//
//    SDL_DestroyWindow(main_window);
//    SDL_Quit();
//    return 0;
//}
