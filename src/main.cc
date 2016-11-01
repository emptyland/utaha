#include "character.h"
#include "datatype.h"
#include "GLFW/glfw3.h"
#include "lodepng.h"
#include "freetype/ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <OpenGL/gl.h>
#include <stdio.h>


struct Controller {
    float scale;
    float x_angle;
    float y_angle;
};

Controller ctl = {0.1, 20, 20};

void FrustumResizeCallback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(0, w, 0, h, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void OnKeyInput(GLFWwindow *window,int key, int code, int action,
                       int mods) {
    switch (key) {
        case GLFW_KEY_A:
            if ((action == GLFW_REPEAT ||
                action == GLFW_PRESS)) {
                ctl.y_angle -= 2;
            }
            break;

        case GLFW_KEY_D:
            if ((action == GLFW_REPEAT ||
                 action == GLFW_PRESS)) {
                ctl.y_angle += 2;
            }
            break;

        case GLFW_KEY_W:
            if ((action == GLFW_REPEAT ||
                 action == GLFW_PRESS)) {
                ctl.x_angle -= 2;
            }
            break;


        case GLFW_KEY_S:
            if ((action == GLFW_REPEAT ||
                 action == GLFW_PRESS)) {
                ctl.x_angle += 2;
            }
            break;

        case GLFW_KEY_R:
            if ((action == GLFW_REPEAT ||
                 action == GLFW_PRESS)) {
                ctl.scale += 0.01;
            }
            break;

        case GLFW_KEY_F:
            if ((action == GLFW_REPEAT ||
                 action == GLFW_PRESS)) {
                ctl.scale -= 0.01;
            }
            break;
    }
}

static void DrawTree(const utaha::Float3 &pos, float size, int tex_id) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glBegin(GL_QUADS);
    {
        glColor3f(1.0, 1.0, 1.0);

        glTexCoord2f(1.0, 1.0);
        glVertex3f(pos.x, pos.y - size * 2, pos.z);

        glTexCoord2f(1.0/2, 1.0);
        glVertex3f(pos.x + size, pos.y - size * 2, pos.z);

        glTexCoord2f(1.0/2, 0.0);
        glVertex3f(pos.x + size, pos.y, pos.z);

        glTexCoord2f(1.0, 0.0);
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

static void DrawRole(const utaha::Float3 &pos, float size, double time,
                     int tex_id) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glBegin(GL_QUADS);
    {
        glColor3f(1.0, 1.0, 1.0);

        glTexCoord2f(0, 0);
        glVertex3f(pos.x, pos.y + size, pos.z);

        glTexCoord2f(1.0/12, 0);
        glVertex3f(pos.x + size, pos.y + size, pos.z);

        glTexCoord2f(1.0/12, 1.0/8);
        glVertex3f(pos.x + size, pos.y, pos.z);

        glTexCoord2f(0, 1.0/8);
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

static void DrawClub(const utaha::Float3 &pos, float size, int tex_id) {


    glEnable(GL_TEXTURE_2D);
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
    glTexCoord2f(0.0, 0.0);     glVertex3f(pos.x, pos.y, pos.z);
    glTexCoord2f(1.0/8, 0.0);   glVertex3f(pos.x + size, pos.y, pos.z);
    glTexCoord2f(1.0/8, 1.0/4); glVertex3f(pos.x + size, pos.y, pos.z + size);
    glTexCoord2f(0.0, 1.0/4);   glVertex3f(pos.x, pos.y, pos.z + size);
//    glTexCoord2f(0.0, 0.0);  glVertex3f(-1, 1, -1);
//    glTexCoord2f(1.0/8, 0.0);  glVertex3f(1, 1, -1);
//    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(1, 1, 1);
//    glTexCoord2f(0.0, 1.0/4);  glVertex3f(-1, 1, 1);

    // right
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 0.0);  glVertex3f(pos.x + size, pos.y, pos.z);
    glTexCoord2f(1.0/8, 0.0);  glVertex3f(pos.x + size, pos.y, pos.z + size);
    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(pos.x + size, pos.y - size, pos.z + size);
    glTexCoord2f(0.0, 1.0/4);  glVertex3f(pos.x + size, pos.y - size, pos.z);
//    glTexCoord2f(0.0, 0.0);  glVertex3f(1, 1, -1);
//    glTexCoord2f(1.0/8, 0.0);  glVertex3f(1, 1, 1);
//    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(1, -1, 1);
//    glTexCoord2f(0.0, 1.0/4);  glVertex3f(1, -1, -1);

    // front
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 0.0);  glVertex3f(pos.x, pos.y - size, pos.z);
    glTexCoord2f(1.0/8, 0.0);  glVertex3f(pos.x + size, pos.y - size, pos.z);
    glTexCoord2f(1.0/8, 1.0/4);  glVertex3f(pos.x + size, pos.y, pos.z);
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

    auto window = glfwCreateWindow(300, 300, "demo", nullptr, nullptr);
    if (!window) {
        return 1;
    }

    glfwSetKeyCallback(window, OnKeyInput);

    glfwMakeContextCurrent(window);
    //glfwSetWindowRefreshCallback(window, RefreshCallback);
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

    auto tree_tex_id = LoadNormal2DTexture("assets/test-tree.png");
    if (tree_tex_id < 0) {
        return 255;
    }

    auto role_tex_id = LoadNormal2DTexture("assets/test-player.png");
    if (role_tex_id < 0) {
        return 255;
    }

    auto character = utaha::CreateASCIICharacter(txt_tex_id);
    character->set_size({0.05, 0.05});

    auto club_size = 2.0;
    while (!glfwWindowShouldClose(window)) {
        auto last_ts = glfwGetTime();

        int w, h;
        //glfwGetWindowSize(window, &w, &h);
        glfwGetFramebufferSize(window, &w, &h);
        //FrustumResizeCallback(window, w, h);

        // TO 3D
        glViewport(0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glFrustum(0, w, 0, h, -100.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        glRotated(ctl.x_angle, -1, 0, 0);

        glPushMatrix();
        glRotated(ctl.y_angle, 0.0, 1.0, 0.0);

        glPushMatrix();
        glScaled(ctl.scale, ctl.scale, ctl.scale);

        DrawClub({-1, 1, -1}, club_size, club_tex_id);
        DrawClub({1, 1, -1}, club_size, club_tex_id);
        DrawClub({-3, 1, -1}, club_size, club_tex_id);
        DrawClub({-3, 1, -3}, club_size, club_tex_id);
        DrawClub({-3, 1, 1}, club_size, club_tex_id);

        DrawTree({-1, 5, -0.5}, club_size, tree_tex_id);
        DrawRole({-3, 1, 0}, club_size, last_ts, role_tex_id);

        glPopMatrix();
        glPopMatrix();
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);

        // TO 2D
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, w, 0, h, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_CULL_FACE);

        glClear(GL_DEPTH_BUFFER_BIT);
        character->set_color({1, 1, 0});
        character->set_position({0, 20, 0});
        character->set_size({20, 20});
        character->Printf("x-angle: %0.2f y-angle: %0.2f", ctl.x_angle,
                          ctl.y_angle);
        character->mutable_position()->y -= 20;
        character->mutable_position()->x  = 0;
        character->Printf("demo v0.1");

        // Undo 2D
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}