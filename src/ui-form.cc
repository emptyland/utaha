#include "ui-form.h"
#include "ui-component.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

namespace utaha {

UIForm::UIForm() {
}

/*virtual*/ UIForm::~UIForm() {
    delete main_menu_;
    main_menu_ = nullptr;

    delete status_bar_;
    status_bar_ = nullptr;

    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}

/*virtual*/ int UIForm::OnClick(UIComponent *sender, bool *is_break) {
    return 0;
}

/*virtual*/ int UIForm::OnDoubleClick(UIComponent *sender, bool *is_break) {
    return 0;
}

/*virtual*/ int UIForm::OnMouseMove(UIComponent *sender, int x, int y,
                                    bool *is_break) {
    return 0;
}
/*virtual*/ int UIForm::OnCommand(UIComponent *sender, int cmd_id, void *param,
                                  bool *is_break) {
    return 0;
}

/*virtual*/ int UIForm::OnInit() {
    return 0;
}

/*virtual*/ void UIForm::OnQuit() {
}

/*virtual*/ void UIForm::OnAfterRender() {
}

/*virtual*/ void UIForm::OnBeforeRender() {
}

/*virtual*/ void UIForm::OnEvent(SDL_Event *e) {
    switch (e->type) {
        case SDL_WINDOWEVENT:
            if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int w = e->window.data1;
                int h = e->window.data2;
                if (main_menu_) {
                    main_menu_->mutable_rect()->w = w;
                }
                if (status_bar_) {
                    status_bar_->mutable_rect()->w = w;
                    status_bar_->mutable_rect()->y = h - status_bar_->rect().h;
                }
            }
            break;
        default:
            break;
    }
}

int UIForm::GetRetainTopH() const {
    return main_menu_ ? main_menu_->rect().h : 0;
}

int UIForm::GetRetainBottomH() const {
    return status_bar_ ? status_bar_->rect().h : 0;
}

bool UIForm::CreateWindow(const char *title, int w, int h) {
    window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, w, h,
                               SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    if (!window_) {
        LOG(ERROR) << "Can not create window! " << SDL_GetError();
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        LOG(ERROR) << "Can not create renderer! " << SDL_GetError();
        return false;
    }
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0xFF);
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    return this->OnInit() == 0;
}

int UIForm::Run() {
    bool quit = false;
    bool is_break = false;
    //bool is_break = false;
    SDL_Event e;
    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            if (main_menu_) {
                main_menu_->OnEvent(&e, &is_break);
            }
            if (status_bar_) {
                status_bar_->OnEvent(&e, &is_break);
            }
            this->OnEvent(&e);
        }

        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer_);

        this->OnAfterRender();
        root_.OnRender(renderer_);
        this->OnBeforeRender();

        if (main_menu_) {
            main_menu_->OnRender(renderer_);
        }
        if (status_bar_) {
            status_bar_->OnRender(renderer_);
        }
        SDL_RenderPresent(renderer_);
    }
    this->OnQuit();
    return 0;
}

} // namespace utaha
