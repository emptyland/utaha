#include "GLFW/glfw3.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    glfwInit();

    auto window = glfwCreateWindow(100, 200, "demo", nullptr, nullptr);
    if (!window) {
        return 1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}