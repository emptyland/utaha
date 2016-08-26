#include "GLFW/glfw3.h"
#include "freetype/ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <OpenGL/gl.h>
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