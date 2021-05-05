#include "engine.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

static hashnameset<font> fonts;
// TODO: better font setup
font *curfont = NULL;

#define FONTTAB (4 * FONTW)
#define TEXTTAB(x) ((int((x) / FONTTAB) + 1.0f) * FONTTAB)

FT_Library ft;
// Inits the font library
bool init_fonts()
{
	if (FT_Init_FreeType(&ft))
	{
		return false;
	}
	return true;
}

// cleans up the font library
void cleanup_fonts()
{
	FT_Done_FreeType(ft);
}

// Gets the atlas texture's size based on the max height/width of a character
// TODO: Inefficient, should pack tightly!
int find_atlas_size(int maxwidth, int maxheight, int numcharacters)
{
	// assume 64 to be the smallest possible
	int size = 64;

	// 4096 is *hopefully* a silly number, so we just cap it there
	// FIXME: If not, tell me!
	while (size < 4096)
	{
		// horizontal width required
		int totalwidth = maxwidth * numcharacters;
		int rows = (totalwidth / size) + 1;
	
		if (rows * maxheight < size)
		{
			// Big enough
			break;
		}

		size *= 2;	
	}

	return size;
}

// This was instrumental in writing:
// https://learnopengl.com/In-Practice/Text-Rendering
void newfont(char *name, char *fp, int *defaultw, int *defaulth)
{
	font *f = &fonts[name];
	if (!f->name)
		f->name = newstring(name);

	f->scale = *defaulth;

	FT_Face face;
	if (FT_New_Face(ft, fp, 0, &face))
	{
		conoutf(CON_ERROR, "freetype could not load font %s", fp);
	}

	FT_Set_Pixel_Sizes(face, *(unsigned int *)defaultw, *(unsigned int *)defaulth);

	// figure out the biggest char
	// TODO: there must be a better way
	int biggestw = *defaultw;
	int biggesth = *defaulth;
	int biggesttop = 0;

	unsigned char c;
	for (c = 0; c < 128; c++) // hehe c++
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			continue; // don't use this invalid character for size calculation
		}

		if (face->glyph->bitmap.width > biggestw)
		{
			biggestw = face->glyph->bitmap.width;
		}

		if (face->glyph->bitmap.rows > biggesth)
		{
			biggesth = face->glyph->bitmap.rows;
		}

		if (face->glyph->bitmap_top > biggesttop)
		{
			biggesttop = face->glyph->bitmap_top;
		}
	}

	// atlas generation, figure out size
	int atlassize = find_atlas_size(biggestw, biggesth, 128);
	int atlaspixels = atlassize * atlassize;

	// contains alpha values
    char* atlasbuffer = new char[atlaspixels];

    // set all atlas pixels to fully transparent
    for(int i = 0; i < atlaspixels; ++i) {
		atlasbuffer[i] = 0;
    }

	f->lineheight = (face->size->metrics.ascender - face->size->metrics.descender) >> 6;

	f->chars.shrink(0);
	f->charoffset = 0;

	// Pre-allocate 128 character definitions
	f->chars.reserve(128);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	// Our position in the atlas
	int atlasx = 0;
	int atlasy = 0;

	int atlasrowsize = atlassize;

	// TODO: glue to the textures system
	unsigned int texid;
	glGenTextures(1, &texid);

	for (c = 0; c < 128; c++) // hehe c++
	{
		font::charinfo &cinfo = f->chars.add();

		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			conoutf(CON_WARN, "Font is missing glpyh %c.", c);
			cinfo.tex = notexture->id;
			cinfo.offsetx = 0;
			cinfo.offsety = 0;
			cinfo.advance = cinfo.w = *defaultw;
			cinfo.h = *defaulth;

			cinfo.texx = 0;
			cinfo.texy = 0;
			cinfo.texw = 1;
			cinfo.texh = 1;

			continue;
		}

		if (atlasx + biggestw > atlassize)
		{ // Loop around
			atlasx = 0;
			atlasy += biggesth + biggesttop;
		}

		// add to the atlas
		int atlasi = atlasrowsize * (atlasy + (biggesttop - face->glyph->bitmap_top)) + atlasx;
		int bitmapsize = face->glyph->bitmap.width * face->glyph->bitmap.rows;
		for (int i = 0; i < bitmapsize; ++i)
		{
			atlasbuffer[atlasi] = face->glyph->bitmap.buffer[i];

			if ( ( (i + 1) % face->glyph->bitmap.width ) == 0 && i != 0)
			{
				// start a new row
	    		atlasi += atlasrowsize - face->glyph->bitmap.width + 1;
			}
			else
			{
				atlasi ++;
			}
		}

		// Texture coordinates
		cinfo.texx = atlasx;
		cinfo.texy = atlasy;
		cinfo.texw = (atlasx + (face->glyph->advance.x >> 6) - face->glyph->bitmap_left);
		cinfo.texh = (atlasy + biggesth);
		// make them in texture percentages instead of pixels
		cinfo.texx /= atlassize;
		cinfo.texy /= atlassize;
		cinfo.texw /= atlassize;
		cinfo.texh /= atlassize;

		cinfo.tex = texid;
		cinfo.offsetx = face->glyph->bitmap_left;
		cinfo.offsety = face->glyph->bitmap_top;
		cinfo.w = face->glyph->bitmap.width;
		cinfo.h = face->glyph->bitmap.rows;
		cinfo.advance = face->glyph->advance.x;

		atlasx += biggestw;

		// conoutf(CON_DEBUG, "%c - %f, %f, %f, %f", c, cinfo.texx, cinfo.texy, cinfo.texw, cinfo.texh);
	}

	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlassize, atlassize, 0, GL_RED,
					GL_UNSIGNED_BYTE, atlasbuffer);

	// Swizzle to alpha
	GLint swizzlemask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzlemask);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FT_Done_Face(face);

	delete[] atlasbuffer;
}

COMMANDN(font, newfont, "ssii");

// sets the curfont
bool setfont(const char *name)
{
	font *f = fonts.access(name);
	if (!f)
		return false;
	curfont = f;
	return true;
}

// A stack to store fonts.
// Ideally you'd push a font to it when switching, so you can go back to the previous font with pop.
static vector<font *> fontstack;

// Pushes the current font to the stack
void pushfont()
{
	fontstack.add(curfont);
}

// Pops the font off of the stack
bool popfont()
{
	if (fontstack.empty())
		return false;
	curfont = fontstack.pop();
	return true;
}

// Returns the resolution we render text at(?)
void gettextres(int &w, int &h)
{
	if (w < MINRESW || h < MINRESH)
	{
		if (MINRESW > w * MINRESH / h)
		{
			h = h * MINRESW / w;
			w = MINRESW;
		}
		else
		{
			w = w * MINRESH / h;
			h = MINRESH;
		}
	}
}

// Sets the width, height values to the charinfo associated with the character, according to the current font.
void font_metrics(char c, float &width, float &height, float &offsetx, float &offsety, float &advance)
{
	font::charinfo cinfo = curfont->chars[c - curfont->charoffset];

	width = cinfo.w;
	height = cinfo.h;

	offsetx = cinfo.offsetx;
	offsety = (curfont->lineheight * 0.5) - cinfo.offsety;

	// For some reason the value here isn't in pixels.
	// WHY? Anyway, we need to bitshift it.
	advance = (cinfo.advance >> 6);
}

// Returns the width that text will take up to render the string.
float text_widthf(const char *str)
{
	float w, h;
	text_boundsf(str, w, h);

	return w;
}

// TODO: What does this do? And why?
void draw_textf(const char *fstr, int left, int top, ...)
{
	defvformatstring(str, top, fstr);
	draw_text(str, left, top);
}

const matrix4x3 *textmatrix = NULL;

// Draws the given character, if the font has it.
static float draw_char(char c, float x, float y, float scale = 1.0)
{
	if (!curfont->chars.inrange(c - curfont->charoffset))
		return 0.0;
	font::charinfo cinfo = curfont->chars[c - curfont->charoffset];

	float w, h, ox, oy, adv;
	font_metrics(c, w, h, ox, oy, adv);

	float x1 = x + scale * ox;
	float y1 = y + scale * oy;
	float x2 = x1 + scale * w;
	float y2 = y1 + scale * h;

	gle::begin(GL_QUADS);
	glBindTexture(GL_TEXTURE_2D, cinfo.tex);
	if (textmatrix) // 3D TEXT
	{
		gle::attrib(textmatrix->transform(vec2(x1, y1)));
		gle::attribf(cinfo.texx, cinfo.texy);
		gle::attrib(textmatrix->transform(vec2(x2, y1)));
		gle::attribf(cinfo.texw, cinfo.texy);
		gle::attrib(textmatrix->transform(vec2(x2, y2)));
		gle::attribf(cinfo.texw, cinfo.texh);
		gle::attrib(textmatrix->transform(vec2(x1, y2)));
		gle::attribf(cinfo.texx, cinfo.texh);
	}
	else // HUD TEXT
	{
		gle::attribf(x1, y1);
		gle::attribf(cinfo.texx, cinfo.texy);
		gle::attribf(x2, y1);
		gle::attribf(cinfo.texw, cinfo.texy);
		gle::attribf(x2, y2);
		gle::attribf(cinfo.texw, cinfo.texh);
		gle::attribf(x1, y2);
		gle::attribf(cinfo.texx, cinfo.texh);
	}

	// // HACK:
	// // show me the fucking texture
	// gle::attribf(0, 0);
	// gle::attribf(0, 0);
	// gle::attribf(512, 0);
	// gle::attribf(1, 0);
	// gle::attribf(512, 512);
	// gle::attribf(1, 1);
	// gle::attribf(0, 512);
	// gle::attribf(0, 1);


	gle::end();

	return scale * adv;
}

// Sets the gle::color to the text colour associated with the code.
static void text_color(char c, char *stack, int size, int &sp, bvec color, int a)
{
	if (c == 's') // save color
	{
		c = stack[sp];
		if (sp < size - 1)
			stack[++sp] = c;
	}
	else
	{
		xtraverts += gle::end();
		if (c == 'r')
		{
			if (sp > 0)
				--sp;
			c = stack[sp];
		} // restore color
		else
			stack[sp] = c;
		switch (c)
		{
			case '0':
				color = bvec(64, 255, 128);
				break; // green: player talk
			case '1':
				color = bvec(96, 160, 255);
				break; // blue: "echo" command
			case '2':
				color = bvec(255, 192, 64);
				break; // yellow: gameplay messages
			case '3':
				color = bvec(255, 64, 64);
				break; // red: important errors
			case '4':
				color = bvec(128, 128, 128);
				break; // gray
			case '5':
				color = bvec(192, 64, 192);
				break; // magenta
			case '6':
				color = bvec(255, 128, 0);
				break; // orange
			case '7':
				color = bvec(255, 255, 255);
				break; // white
			case '8':
				color = bvec(96, 240, 255);
				break; // cyan
					   // provided color: everything else
		}
		gle::color(color, a);
	}
}

// The character visible in text(?)
// Based on YellowBerryHN's solution; https://github.com/ACBob/Bube/pull/1#issuecomment-808835390
int text_visible(const char *str, float hitx, float hity, int maxwidth)
{
	float y = 0, x = 0, scale = curfont->scale / float(curfont->defaulth);
	int i;
	for (i = 0; str[i]; i++)
	{
		int c = uchar(str[i]);
		if (c == '\t')
		{
			x = TEXTTAB(x);
			if (y + FONTH > hity && x >= hitx)
				return i;
		}
		else if (c == ' ')
		{
			x += scale * curfont->defaultw;
			if (y + FONTH > hity && x >= hitx)
				return i;
		}
		else if (c == '\n')
		{
			if (y + FONTH > hity)
				return i;
			x = 0;
			y += FONTH;
		}
		else if (c == '\f')
		{
			if (str[i + 1])
			{
				i++;
			}
		}
		else if (curfont->chars.inrange(c - curfont->charoffset))
		{
			float cw = scale * curfont->chars[c - curfont->charoffset].advance;
			if (cw <= 0)
				continue;
			if (maxwidth != -1)
			{
				int j = i;
				float w = cw;
				for (; str[i + 1]; i++)
				{
					int c = uchar(str[i + 1]);
					if (c == '\f')
					{
						if (str[i + 2])
							i++;
						continue;
					}
					if (i - j > 16)
						break;
					if (!curfont->chars.inrange(c - curfont->charoffset))
						break;
					float cw = scale * curfont->chars[c - curfont->charoffset].advance;
					if (cw <= 0 || w + cw > maxwidth)
						break;
					w += cw;
				}
				if (x + w > maxwidth && j != 0)
				{
					if (y + FONTH > hity)
						return j - 1;
					x = 0;
					y += FONTH;
				}
				for (; j <= i; j++)
				{
					int c = uchar(str[j]);
					if (c == '\f')
					{
						if (str[j + 1])
						{
							j++;
						}
					}
					else
					{
						float cw = scale * curfont->chars[c - curfont->charoffset].advance;
						x += cw;
						if (y + FONTH > hity && x >= hitx)
							return j;
					}
				}
			}
			else
			{
				x += cw;
				if (y + FONTH > hity && x >= hitx)
					return i;
			}
		}
	}
	return i;
}

// TODO:
// inverse of text_visible
void text_posf(const char *str, int cursor, float &cx, float &cy, int maxwidth)
{
	cx = cy = 0;
}

// Sets the width, height variables to the amount of space that the string would occupy, if rendered.
void text_boundsf(const char *str, float &width, float &height, int maxwidth)
{
	width = 0.0;
	float usewidth = 0.0;
	height = curfont->lineheight;

	int i;
	for (i = 0; str[i]; i++)
	{
		char c = uchar(str[i]);

		float w, h, ox, oy, adv;
		font_metrics(c, w, h, ox, oy, adv);

		// Test for special characters, like newlines, as they don't take up space.
		if (c == '\r' || c == '\n' || usewidth >= maxwidth)
		{
			height += (h + oy);

			if (usewidth > width)
			{
				width = usewidth;
			}
			usewidth = 0.0;

			continue;
		}
		else if (c == '\f') // Colour code doesn't take up any space.
		{
			continue;
		}
		else if (c == '\t') // tab
		{
			width += FONTTAB;
			continue;
		}

		width += (w + ox + adv);
	}

	if (usewidth > width)
	{
		width = usewidth;
	}

	if (maxwidth != -1)
		width = maxwidth;
}

// Draws the given string at the x (left), y (top) coords
void draw_text(const char *str, int left, int top, int r, int g, int b, int a, int cursor, int maxwidth)
{
	bvec color(r, g, b);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Gah! Damn you cube 2!
	if (a < 0)
	{
		a = -a;
	}

	gle::color(color, a);
	gle::defvertex(2);

	gle::deftexcoord0();

	float x = left;
	float y = top;

	float cx = x; // cursor x
	float cy = y; // cursor y

	float maxx;

	if (maxwidth != -1)
		maxx = x + maxwidth;
	else
		maxx = screenw;

	char colorstack[10];
	colorstack[0] = 'c'; // indicate user color

	int i;
	for (i = 0; str[i]; i++)
	{

		char c = uchar(str[i]);

		if (i == cursor) // this is the cursor pos!
		{
			cx = x;
			cy = y;
		}

		// Test for special characters, like newlines
		if (c == '\r' || c == '\n' || x > maxx)
		{
			x = left;
			y += curfont->lineheight;
			continue;
		}
		else if (c == '\f') // Colouring
		{
			int cpos = 0;
			if (str[i + 1])
			{
				text_color(str[i + 1], colorstack, sizeof(colorstack), cpos, color, a);
				i++; // skip colour number
			}
			continue;
		}
		else if (c == '\t') // tab
		{
			x += FONTTAB;
			continue;
		}

		x += (int)draw_char(c, x, y, 1.0);
	}

	if (cursor >= 0 && (totalmillis / 250) & 1)
	{
		gle::color(color, a);
		if (maxwidth != -1 && cx >= maxwidth)
		{
			cx = 0;
			cy += FONTH;
		}
		draw_char('_', cx, cy, 1.0);
	}
}

// Unimplemented.
// Reloads fonts from the file.
void reloadfonts()
{
}