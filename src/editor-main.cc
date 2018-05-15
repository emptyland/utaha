#include "ui-form.h"
#include "on-exit-scope.h"
#include SDL_H
#include SDL_IMAGE_H
#include SDL_TTF_H
#include <stdio.h>

namespace utaha {
    extern UIForm *CreateEditorForm();
}

int main(int argc, char *argv[]) {
    FLAGS_alsologtostderr = 1;
    ::google::InitGoogleLogging(argv[0]);

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    utaha::OnExitScope on_exit(utaha::ON_EXIT_SCOPE_INITIALIZER);

    std::unique_ptr<utaha::UIForm> form(utaha::CreateEditorForm());
    if (!form->CreateWindow("Utaha-Editor", 800, 600)) {
        return -1;
    }
    auto result = form->Run();
    TTF_Quit();
    SDL_Quit();
    return result;
}

