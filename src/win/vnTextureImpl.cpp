#include "vnTextureImpl.h"

_vn_begin

TextureImpl::TextureImpl(const vector2i& _size)
: Texture(_size)
, m_texture(0)
{

}

TextureImpl::~TextureImpl() {
	if (m_texture) {
		m_texture->Release();
		m_texture = 0;
	}
}

bool TextureImpl::updatePixels(const vector2i &offset, const vector2i &size, const color_t *pixels) {
	if (size.x <= 0 || size.y <= 0) {
		return false;
	}
	RECT rc = { offset.x, offset.y, offset.x + size.x, offset.y + size.y };

	D3DLOCKED_RECT rect;
	HRESULT hr = m_texture->LockRect(0, &rect, &rc, 0);
	if (FAILED(hr)) return false;

	u32 pitch = (u32)rect.Pitch / sizeof(u32);

	u32* dest = (u32*)rect.pBits;
	const u32 *src = (const u32 *)pixels;

	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			dest[x] = src[x];
		}
		dest += pitch;
		src += size.x;
	}	
	m_texture->UnlockRect(0);
	return true;
}

bool TextureImpl::_init(IDirect3DDevice9* device, const void *pixels) {
	if (m_size.x <= 0 || m_size.y <= 0) {
		return false;
	}
	device->CreateTexture(m_size.x, m_size.y, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texture, NULL);
	if (!m_texture) {
		return false;
	}
	if (!pixels) {
		return true;
	}
	D3DLOCKED_RECT rect;
	HRESULT hr = m_texture->LockRect(0, &rect, NULL, 0);
	if (FAILED(hr)) return false;

	u32 pitch = (u32)rect.Pitch / sizeof(u32);

	u32* dest = (u32*)rect.pBits;
	const u32 *src = (const u32 *)pixels;

	for (int y = 0; y < m_size.y; ++y) {
		for (int x = 0; x < m_size.x; ++x) {
			dest[x] = src[x]; 
		}
		dest += pitch;
		src += m_size.x;
	}
	m_texture->UnlockRect(0);
	return true;
}



bool TextureImpl::createTexture(IDirect3DDevice9* device) {
	device->CreateTexture(m_size.x, m_size.y, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texture, NULL);
	//m_size = vector2i(width, height);
	if (!m_texture)	{
		return false;
	}
	return true;
}

u32 * TextureImpl::lock(u32& pitch, bool readOnly)
{
	D3DLOCKED_RECT locked;
	DWORD dwFlag = 0;
	if (readOnly) {
		dwFlag |= D3DLOCK_READONLY;
	}
	HRESULT hr = m_texture->LockRect(0, &locked, NULL, dwFlag);
	if (FAILED(hr))
		return 0;
	pitch = (u32)locked.Pitch / sizeof(u32);
	return (u32 *)locked.pBits;
}

u32* TextureImpl::lock(u32 left, u32 top, u32 right, u32 bottom, u32 &pitch, bool readOnly){
	D3DLOCKED_RECT locked;
	DWORD dwFlag = 0;
	if (readOnly) {
		dwFlag |= D3DLOCK_READONLY;
	}
	RECT rc = { left, top, right, bottom };
	HRESULT hr = m_texture->LockRect(0, &locked, &rc, dwFlag);
	if (FAILED(hr))
		return 0;
	pitch = (u32)locked.Pitch / sizeof(u32);
	return (u32 *)locked.pBits;
}

void TextureImpl::unlock() {
	m_texture->UnlockRect(0);
}



_vn_end