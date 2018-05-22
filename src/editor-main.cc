#include "ui-form.h"
#include "on-exit-scope.h"
#include "universal-profile.h"
#include "glog/logging.h"
#include SDL_H
#include SDL_IMAGE_H
#include SDL_TTF_H
#include <stdio.h>

namespace utaha {
    extern UIForm *CreateEditorForm(const UniversalProfile *profile);
}

#if defined(UTAHA_OS_WINDOWS)
#include <Windows.h>
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
    FLAGS_logtostderr = false;
    FLAGS_log_dir = ".\\logs";
    ::google::InitGoogleLogging("utaha-editor");

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP;
    if (!(IMG_Init(img_flags) & img_flags)) {
        LOG(FATAL) << "Can not init IMG library." << IMG_GetError();
    }

    utaha::OnExitScope on_exit(utaha::ON_EXIT_SCOPE_INITIALIZER);

    std::unique_ptr<utaha::UIForm> form(utaha::CreateEditorForm());
    if (!form->OpenWindow("Utaha-Editor", 800, 600)) {
        return -1;
    }
    auto result = form->Run();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return result;
}
#else
int main(int argc, char *argv[]) {
    FLAGS_alsologtostderr = 1;
    ::google::InitGoogleLogging(argv[0]);

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    utaha::OnExitScope on_exit(utaha::ON_EXIT_SCOPE_INITIALIZER);

    utaha::UniversalProfile profile;
    std::string err;
    profile.LoadFromFile("res/profile.lua", &err);
    if (!err.empty()) {
        LOG(FATAL) << "Can not load profile file!" << err;
    }

    std::unique_ptr<utaha::UIForm> form(utaha::CreateEditorForm(&profile));
    if (!form->OpenWindow("Utaha-Editor", 1024, 768)) {
        return -1;
    }
    auto result = form->Run();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return result;
}
#endif
