#include "engine.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

static hashnameset<font> fonts;
// TODO: better font setup
font *curfont = NULL;

FT_Library ft;
bool init_fonts()
{
    if(FT_Init_FreeType(&ft))
    {
        return false;
    }
    return true;
}

// This was instrumental in writing:
// https://learnopengl.com/In-Practice/Text-Rendering
void newfont(char *name, char *fp, int *defaultw, int *defaulth)
{
    font *f = &fonts[name];
    if (!f->name) f->name = newstring(name);

    FT_Face face;
    if (FT_New_Face(ft, fp, 0, &face))
    {
        conoutf(CON_ERROR, "freetype could not load font %s", fp);
    }

    FT_Set_Pixel_Sizes(face, *(unsigned int*)defaultw, *(unsigned int*)defaulth);

    f->chars.shrink(0);

    unsigned char c;
    for (c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            conoutf(CON_WARN, "Font is missing glpyh %c.", c);
            continue;
        }

        // TODO: glue to the textures system
        unsigned int texid;
        glGenTextures(1, &texid);
        glBindTexture(GL_TEXTURE_2D, texid);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        font::charinfo cinfo = f->chars.add();
        cinfo.tex = texid;
        cinfo.x = face->glyph->bitmap_left;
        cinfo.y = face->glyph->bitmap_top;
        cinfo.w = face->glyph->bitmap.width;
        cinfo.h = face->glyph->bitmap.rows;
    }

    FT_Done_Face(face);
}

void fontoffset(char *c)
{
}

void fontscale(int *scale)
{
}

void fonttex(char *s)
{
}

void fontchar(int *x, int *y, int *w, int *h, int *offsetx, int *offsety, int *advance)
{
}

void fontskip(int *n)
{
}

COMMANDN(font, newfont, "ssii");
COMMAND(fontoffset, "s");
COMMAND(fontscale, "i");
COMMAND(fonttex, "s");
COMMAND(fontchar, "iiiiiii");
COMMAND(fontskip, "i");

void fontalias(const char *dst, const char *src)
{
}

COMMAND(fontalias, "ss");

bool setfont(const char *name)
{
    font *f = fonts.access(name);
    if(!f) return false;
    curfont = f;
    return true;
}

static vector<font *> fontstack;

void pushfont()
{
}

bool popfont()
{
    return true;
}

void gettextres(int &w, int &h)
{
}

float text_widthf(const char *str) 
{
    return 16.0;
}

void tabify(const char *str, int *numtabs)
{
}

COMMAND(tabify, "si");
    
void draw_textf(const char *fstr, int left, int top, ...)
{
}

const matrix4x3 *textmatrix = NULL;

static float draw_char(Texture *&tex, int c, float x, float y, float scale)
{
    return 0.0;
}

//stack[sp] is current color index
static void text_color(char c, char *stack, int size, int &sp, bvec color, int a) 
{
    if(c=='s') // save color
    {   
        c = stack[sp];
        if(sp<size-1) stack[++sp] = c;
    }
    else
    {
        xtraverts += gle::end();
        if(c=='r') { if(sp > 0) --sp; c = stack[sp]; } // restore color
        else stack[sp] = c;
        switch(c)
        {
            case '0': color = bvec( 64, 255, 128); break;   // green: player talk
            case '1': color = bvec( 96, 160, 255); break;   // blue: "echo" command
            case '2': color = bvec(255, 192,  64); break;   // yellow: gameplay messages 
            case '3': color = bvec(255,  64,  64); break;   // red: important errors
            case '4': color = bvec(128, 128, 128); break;   // gray
            case '5': color = bvec(192,  64, 192); break;   // magenta
            case '6': color = bvec(255, 128,   0); break;   // orange
            case '7': color = bvec(255, 255, 255); break;   // white
            case '8': color = bvec( 96, 240, 255); break;   // cyan
            // provided color: everything else
        }
        gle::color(color, a);
    } 
}

int text_visible(const char *str, float hitx, float hity, int maxwidth)
{
    return 0;
}

//inverse of text_visible
void text_posf(const char *str, int cursor, float &cx, float &cy, int maxwidth) 
{
}

void text_boundsf(const char *str, float &width, float &height, int maxwidth)
{
}

void draw_text(const char *str, int left, int top, int r, int g, int b, int a, int cursor, int maxwidth) 
{
    bvec color(r,g,b);

    loopi(sizeof(str))
    {
        font::charinfo cinfo = curfont->chars[i];

        float x = left;
        float y = top;

        float w = cinfo.w;
        float h = cinfo.h;

        float x1 = left + w;
        float y1 = top + h;

        glBindTexture(GL_TEXTURE_2D, cinfo.tex);

        gle::attribf(x, y); gle::attribf(0,0);
        gle::attribf(x1, y); gle::attribf(1,0);
        gle::attribf(x1, y1); gle::attribf(1,1);
        gle::attribf(x, y1); gle::attribf(0,1);
    }
}

void reloadfonts()
{
}