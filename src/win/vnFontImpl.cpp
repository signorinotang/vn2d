#include "vnFontImpl.h"
#include "vnFileManager.h"
#include "vnLog.h"

_vn_begin

FT_Library FontImpl::s_ftLibrary = 0;

FontImpl::FontImpl()
: m_scale(1)
, m_height(0)
, m_heightInPixel(0)
, m_ftStream({}){
	m_ftStream.read = ftRead;
	m_ftStream.close = ftClose;
}

FontImpl::~FontImpl() {
	if (m_ftFace && s_ftLibrary) {
		FT_Done_Face(m_ftFace);
	}

	if (m_ftStream.descriptor.pointer) {
		static_cast<FileStream *>(m_ftStream.descriptor.pointer)->close();
	}
}

bool FontImpl::ftInit() {
	if (FT_Init_FreeType(&s_ftLibrary)) {
		return false;
	}
	return true;
}

void FontImpl::ftDone() {
	if (s_ftLibrary) {
		FT_Done_FreeType(s_ftLibrary);
		s_ftLibrary = 0;
	}
}

unsigned long FontImpl::ftRead(FT_Stream stream, unsigned long offset, unsigned char *buffer, unsigned long count) {
	FileStream *fs = (FileStream *)stream->descriptor.pointer;
	fs->seek(offset);
	return fs->read(buffer, count);
}

void FontImpl::ftClose(FT_Stream stream) {
	FileStream *fs = (FileStream *)stream->descriptor.pointer;
	fs->close();
	stream->descriptor.pointer = 0;
}

f32 FontImpl::height() const {
	return m_height;
}

bool FontImpl::init(const FilePath& file, u32 size, f32 scale) {



	FileStream* fs = FileManager::instance().open(file);
	if (!fs) {
		return false;
	}
	m_scale = scale;

	m_ftStream.descriptor.pointer = fs;
	m_ftStream.size = (unsigned long)fs->size();
	FT_Open_Args args = { FT_OPEN_STREAM };
	args.stream = &m_ftStream;

	if (FT_Open_Face(s_ftLibrary, &args, 0, &m_ftFace)) {
		fs->close();
		return false;
	}

	//s32 height = (s32)((f32)((u32)(size * scale) << 6) * m_ftFace->units_per_EM / m_ftFace->height / 64.f);

	FT_Select_Charmap(m_ftFace, FT_ENCODING_UNICODE);
	FT_Set_Pixel_Sizes(m_ftFace, size * scale, 0);

	m_heightInPixel = (f32)(m_ftFace->size->metrics.height >> 6);
	m_baseLine = (f32)(m_ftFace->size->metrics.ascender >> 6);

	m_height = m_heightInPixel / m_scale;
	_initTextureAndCells(kTexSize, m_heightInPixel);
	for (GlyphList::iterator it = m_glyphList.begin(); it != m_glyphList.end(); ++it) {
		GlyphEx *glyph = *it;
		glyph->size.y = m_height;
	}

	return true;
}



void FontImpl::_build(const str32& code, CellBasedFont::GlyphEx* glyph) {

	FT_UInt idx = FT_Get_Char_Index(m_ftFace, code.front());
	glyph->valid = (idx != 0);
	FT_Load_Glyph(m_ftFace, idx, FT_LOAD_RENDER);
	FT_GlyphSlot slot = m_ftFace->glyph;

	vector2i size(slot->bitmap.width, slot->bitmap.rows);

	if (!size.x || !size.y) {
		return;
	}

	u32 *pixels = vnmalloc(u32, size.area());
	u32 *dest = pixels;

	vnassert(slot->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY);

	u8 *src = slot->bitmap.buffer;
	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			*dest++ = (0x00FFFFFF | (src[x] << 24));
		}
		src += slot->bitmap.pitch;
	}

	m_texture->updatePixels(glyph->pos, size, (color_t*)pixels);
	vnfree(pixels);

	glyph->size.x = (real)(slot->advance.x >> 6) / m_scale;
	glyph->bounds.min_corner.set((real)slot->bitmap_left, m_baseLine - (real)slot->bitmap_top);
	glyph->bounds.max_corner = glyph->bounds.min_corner + size;
	glyph->bounds /= m_scale;

	glyph->texcoords[2] = vector2f(glyph->pos + size) / kTexSize;
	glyph->texcoords[1].x = glyph->texcoords[2].x;
	glyph->texcoords[3].y = glyph->texcoords[2].y;
}

















_vn_end



