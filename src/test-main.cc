#include "on-exit-scope.h"
#include "gtest/gtest.h"
#include "glog/logging.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    FLAGS_alsologtostderr = 1;

    ::google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);

    ::utaha::OnExitScope on_exit(::utaha::ON_EXIT_SCOPE_INITIALIZER);
    int result = RUN_ALL_TESTS();

    TTF_Quit();
    SDL_Quit();
    return result;
}
