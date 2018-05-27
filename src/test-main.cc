#include "on-exit-scope.h"
#include "gtest/gtest.h"
#include "glog/logging.h"
#include SDL_H
#include SDL_IMAGE_H
#include SDL_TTF_H


// --gtest_filter=*.*
#if defined(UTAHA_OS_WINDOWS)

int wmain(int argc, wchar_t *argv[], wchar_t *envp[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    FLAGS_alsologtostderr = 1;

    ::google::InitGoogleLogging("utaha-tests");
    ::testing::InitGoogleTest(&argc, argv);

    ::utaha::OnExitScope on_exit(::utaha::ON_EXIT_SCOPE_INITIALIZER);
    int result = RUN_ALL_TESTS();

    TTF_Quit();
    SDL_Quit();
    system("pause");
    return result;
}

#else

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

#endif