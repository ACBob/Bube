#include "engine.h"

void newfont(char *name, char *tex, int *defaultw, int *defaulth)
{
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
}

void reloadfonts()
{
}