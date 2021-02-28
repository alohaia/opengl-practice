#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

class Font
{
private:
public:
    FT_Library ft;
    FT_Face face;
    //Private members here.
    Font(const char * font_path);
    ~Font();

    void setSize(unsigned int width, unsigned int hight);
};

Font::Font(const char * font_path)
{
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    // A FreeType face contains a set of font shape
    if (FT_New_Face(ft, "/usr/share/fonts/nerd-fonts/JetBrains Mono Regular Nerd Font Complete Mono.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
}

void Font::setSize(unsigned int width, unsigned int hight)
{
    FT_Set_Pixel_Sizes(face, 0, 48);
}

Font::~Font()
{
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
