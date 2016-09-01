#include "datatype.h"
#include "GLFW/glfw3.h"
#include "lodepng.h"
#include "freetype/ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <OpenGL/gl.h>
#include <stdio.h>

void RefreshCallback(GLFWwindow *window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(0, w, 0, h, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void DrawClub(const utaha::Float3 &pos, int tex_id) {


    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_BLEND);
////    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

    glBindTexture(GL_TEXTURE_2D, tex_id);
    glBegin(GL_QUADS);

    //        // back
    //        glColor3f(1.0, 1.0, 1.0);
    //        glVertex3f(-1, -1, 1);
    //        glVertex3f(1, -1, 1);
    //        glVertex3f(1, 1, 1);
    //        glVertex3f(-1, 1, 1);

    // top
    glColor3f(1.0, 1.0, 1.0);
    // -1, 1, -1
    glTexCoord2f(0.0, 0.0);     glVertex3f(pos.x,     pos.y, pos.z);
    glTexCoord2f(1.0/8, 0.0);   glVertex3f(pos.x + 2, pos.y, pos.z);
    glTexCoord2f(1.0/8, 1.0/4); glVertex3f(pos.x + 2, pos.y, pos.z + 2);
    glTexCoord2f(0.0, 1.0/4);   glVertex3f(pos.x,     pos.y, pos.z + 2);
//    glTexCoord2f(0.0, 0.0);  glVertex3f(-1, 1, -1);
//    glTexCoord2f(1.0/8, 0.0);  glVertex3f(1, 1, -1);
//    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(1, 1, 1);
//    glTexCoord2f(0.0, 1.0/4);  glVertex3f(-1, 1, 1);

    // right
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 0.0);  glVertex3f(pos.x + 2, pos.y, pos.z);
    glTexCoord2f(1.0/8, 0.0);  glVertex3f(pos.x + 2, pos.y, pos.z + 2);
    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(pos.x + 2, pos.y - 2, pos.z + 2);
    glTexCoord2f(0.0, 1.0/4);  glVertex3f(pos.x + 2, pos.y - 2, pos.z);
//    glTexCoord2f(0.0, 0.0);  glVertex3f(1, 1, -1);
//    glTexCoord2f(1.0/8, 0.0);  glVertex3f(1, 1, 1);
//    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(1, -1, 1);
//    glTexCoord2f(0.0, 1.0/4);  glVertex3f(1, -1, -1);

    // front
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 0.0);  glVertex3f(pos.x, pos.y - 2, pos.z);
    glTexCoord2f(1.0/8, 0.0);  glVertex3f(pos.x + 2, pos.y - 2, pos.z);
    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(pos.x + 2, pos.y, pos.z);
    glTexCoord2f(0.0, 1.0/4);  glVertex3f(pos.x, pos.y, pos.z);

//    glTexCoord2f(0.0, 0.0);  glVertex3f(-1, -1, -1);
//    glTexCoord2f(1.0/8, 0.0);  glVertex3f(1, -1, -1);
//    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(1, 1, -1);
//    glTexCoord2f(0.0, 1.0/4);  glVertex3f(-1, 1, -1);
    glEnd();

    //glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);


    //
}

void DrawFont(int font_tex_id) {
    //glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, font_tex_id);
    glColor3f(0, 1, 1);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, -1);
        glTexCoord2f(0.0, 1.0); glVertex3f(0, 1, -1);
        glTexCoord2f(1.0, 1.0); glVertex3f(1, 1, -1);
        glTexCoord2f(1.0, 0.0); glVertex3f(1, 0, -1);
    }
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
}

int LoadNormal2DTexture(const char *file_name) {
    // laod texture
    std::vector<unsigned char> image;
    unsigned int width, height;
    auto err = lodepng::decode(image, width, height, file_name);
    if (err) {
        printf("load png fail\n");
        return -1;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, &image[0]);
    return tex_id;
}

int main(int argc, char *argv[]) {
    glfwInit();

    auto window = glfwCreateWindow(400, 400, "demo", nullptr, nullptr);
    if (!window) {
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowRefreshCallback(window, RefreshCallback);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    glfwSwapInterval(60);

    auto txt_tex_id = LoadNormal2DTexture("assets/bitmap-font.png");
    if (txt_tex_id < 0) {
        return 255;
    }

    auto club_tex_id = LoadNormal2DTexture("assets/club-1.png");
    if (club_tex_id < 0) {
        return 255;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        glRotated(20, -1, 0, 0);

        glPushMatrix();
        glRotated(20, 0.0, 1.0, 0.0);

        glPushMatrix();
        glScaled(0.2, 0.2, 0.2);

        DrawClub({-1, 1, -1}, club_tex_id);
        DrawClub({1, 1, -1}, club_tex_id);
        DrawClub({-3, 1, -1}, club_tex_id);
        DrawClub({-3, 1, -3}, club_tex_id);
        DrawClub({-3, 1, 1}, club_tex_id);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();

        DrawFont(txt_tex_id);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}