#ifndef __vn2d__vnFontImpl__
#define __vn2d__vnFontImpl__

#include "vnCellBasedFont.h"
#include "vnFilePath.h"
#include <ft2build.h>
#include FT_FREETYPE_H

_vn_begin

class FontImpl : public CellBasedFont {
public:
	FontImpl(); 
	virtual ~FontImpl();

	bool init(const FilePath& file, u32 size, f32 scale);

	static bool ftInit(); //init free type library
	static void ftDone();
	static unsigned long ftRead(FT_Stream stream, unsigned long offset, unsigned char *buffer, unsigned long count);
	static void ftClose(FT_Stream stream);


	virtual f32 height() const;

protected:
	enum { kTexSize = 1024 };
	virtual void _build(const str32& code, GlyphEx* glyph);
	
	f32 m_height;
	f32 m_scale;
	u32 m_heightInPixel;
	//
	s32 m_baseLine;

	static FT_Library s_ftLibrary;
	FT_Face m_ftFace;
	FT_StreamRec_ m_ftStream;

};
_vn_end


#endif