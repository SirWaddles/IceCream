#ifndef _FONT_HEADER_
#define _FONT_HEADER_

#include "graphics/texture.h"

class TFont {
public:
	TFont(char *path, int size);
	~TFont();

	unsigned int GetFontName();
	unsigned int GetTexName();
	unsigned int GetFontWidth();
	bool GetTexAlpha();
private:
	Texture fontImg;
	unsigned int fontLoc;
	unsigned int fontWidth;
};

namespace FontMan {
	void BuildFont(char *name, char *path, int size);
	void ClearFont(const char *name);
	void ClearAllFonts();
	TFont *GetFont(char *name);
}


#endif