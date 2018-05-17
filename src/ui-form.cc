#include "ui-form.h"
#include "ui-flat-menu-group.h"
#include "ui-flat-status-bar.h"
#include "ui-component.h"
#include SDL_H
#include SDL_IMAGE_H
#include SDL_TTF_H

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
    if (main_menu_) {
        main_menu_->OnRender(renderer_);
    }
    if (status_bar_) {
        status_bar_->OnRender(renderer_);
    }
}

/*virtual*/ void UIForm::OnEvent(SDL_Event *e, bool *is_break) {
    if (main_menu_) {
        if (main_menu_->OnEvent(e, is_break) == 1) {
            *is_break = true;
            return;
        }
    }
    if (status_bar_) {
        status_bar_->OnEvent(e, is_break);
    }

    switch (e->type) {
        case SDL_WINDOWEVENT:
            if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int w = e->window.data1;
                int h = e->window.data2;
//                if (main_menu_) {
//                    main_menu_->mutable_rect()->w = w;
//                }
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

bool UIForm::OpenWindow(const char *title, int w, int h) {
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
    SDL_Event e;

    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }

            bool is_break = false;
            OnEvent(&e, &is_break);
        }

        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer_);

        OnAfterRender();
        root_.OnRender(renderer_);
        OnBeforeRender();
        SDL_RenderPresent(renderer_);
    }
    OnQuit();
    return 0;
}

} // namespace utaha
