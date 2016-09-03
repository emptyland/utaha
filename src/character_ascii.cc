#include "character.h"
#include "datatype.h"
#include "base.h"
#include "glog/logging.h"
#include <OpenGL/gl.h>
#include <stdio.h>

namespace utaha {

class CharacterASCII : public Character8 {
public:
    const static int kNumWidthChar  = 16;
    const static int kNumHeightChar = 16;

    CharacterASCII(int tex_id)
        : tex_id_(tex_id) {
        DCHECK_GE(tex_id, 0) << "invalid tex_id";
    }

    virtual ~CharacterASCII() override {}

    virtual void Vprintf(const char *fmt, va_list ap) override {
        vsnprintf(buf_, arraysize(buf_), fmt, ap);
        DrawFonts();
    }

private:
    void DrawFonts() {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture(GL_TEXTURE_2D, tex_id_);
        glColor3f(color().x, color().y, color().z);
        glBegin(GL_QUADS);
        for (int i = 0; buf_[i]; i++) {
            DrawFont(buf_[i]);
            mutable_position()->x += size().x;
        }
        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }

    void DrawFont(int ch) {
        if (ch <= 0 || ch > 127) {
            ch = 0;
        }

        Float2 char_size{1.0/kNumWidthChar, 1.0/kNumHeightChar};

        Float2 char_pos;
        char_pos.x = (ch % kNumWidthChar)  * char_size.x;
        char_pos.y = (ch / kNumHeightChar) * char_size.y;

        glTexCoord2f(char_pos.x, char_pos.y + char_size.y);
        glVertex3f(position().x, position().y, position().z);

        glTexCoord2f(char_pos.x + char_size.x, char_pos.y + char_size.y);
        glVertex3f(position().x + size().x, position().y, position().z);

        glTexCoord2f(char_pos.x + char_size.x, char_pos.y);
        glVertex3f(position().x + size().x, position().y + size().y,
                   position().z);

        glTexCoord2f(char_pos.x, char_pos.y);
        glVertex3f(position().x, position().y + size().y, position().z);
    }

    int tex_id_;
    char buf_[128];
};

Character8 *CreateASCIICharacter(int tex_id) {
    return new CharacterASCII(tex_id);
}

} // namespace utaha