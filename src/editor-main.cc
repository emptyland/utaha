#include "ui-form.h"
#include "on-exit-scope.h"
#include SDL_H
#include SDL_IMAGE_H
#include SDL_TTF_H
#include <stdio.h>

namespace utaha {
    extern UIForm *CreateEditorForm();
}

#if defined(UTAHA_OS_WINDOWS)
#include <Windows.h>
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	FLAGS_alsologtostderr = 1;
	::google::InitGoogleLogging("utaha-editor");

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	utaha::OnExitScope on_exit(utaha::ON_EXIT_SCOPE_INITIALIZER);

	std::unique_ptr<utaha::UIForm> form(utaha::CreateEditorForm());
	if (!form->OpenWindow("Utaha-Editor", 800, 600)) {
		return -1;
	}
	auto result = form->Run();
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
#endif