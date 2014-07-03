//
//  vnUIImage.h
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUIImage_h
#define vn2d_vnUIImage_h

#include "vnUIFactory.h"
#include "vnPicture.h"
#include "vnColor.h"


_vn_begin

class UIRenderer;

class _vn_2d_api UIImage : public RefCounted {
public:
	virtual UIImage * clone() const = 0;
	
	virtual void update(f32 deltaTime);
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void setLocation(const aabox2f &rect);
	virtual void init(const TreeDataObject *object);
	
	virtual vector2f calculateMinimumSize() const;
};

typedef RefCountedPtr<UIImage> UIImagePtr;

class _vn_gfx_api UIImageEmpty : public UIImage {
	VN_DECL_UI_IMAGE()
public:
	virtual UIImageEmpty * clone() const;
};

class _vn_gfx_api UIImage0 : public UIImage {
	VN_DECL_UI_IMAGE()
public:
	virtual UIImage0 * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	virtual void setLocation(const aabox2f &rect);
protected:
	aabox2f m_rect;
};

class _vn_gfx_api UIImage1 : public UIImage0 {
	VN_DECL_UI_IMAGE()
public:
	virtual UIImage1 * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void init(const TreeDataObject *object);
	
	void setPicture(const PicturePtr &picture);
	const PicturePtr & getPicture() const;
	
protected:
	PicturePtr m_picture;
};

class _vn_gfx_api UIImage3V : public UIImage {
	VN_DECL_UI_IMAGE()
public:
	UIImage3V();
	virtual UIImage3V * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void setLocation(const aabox2f &rect);
	virtual void init(const TreeDataObject *object);
	virtual vector2f calculateMinimumSize() const;
	
	void setPicture(size_t index, const PicturePtr &picture);
	const PicturePtr & getPicture(size_t index) const;
	
	void setHeightTop(f32 height);
	f32 getHeightTop() const;
	
	void setHeightBottom(f32 height);
	f32 getHeightBottom() const;
	
protected:
	aabox2f m_rect[3];
	PicturePtr m_picture[3];
	f32 m_heightTop, m_heightBottom;
};

class _vn_gfx_api UIImage3H : public UIImage {
	VN_DECL_UI_IMAGE()
public:
	UIImage3H();
	virtual UIImage3H * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void setLocation(const aabox2f &rect);
	virtual void init(const TreeDataObject *object);
	virtual vector2f calculateMinimumSize() const;
	
	void setPicture(size_t index, const PicturePtr &picture);
	const PicturePtr & getPicture(size_t index) const;
	
	void setWidthLeft(f32 width);
	f32 getWidthLeft() const;
	
	void setWidthRight(f32 width);
	f32 getWidthRight() const;
	
protected:
	aabox2f m_rect[3];
	PicturePtr m_picture[3];
	f32 m_widthLeft, m_widthRight;
};

class _vn_gfx_api UIImage9 : public UIImage {
	VN_DECL_UI_IMAGE()
public:
	UIImage9();
	virtual UIImage9 * clone() const;
	virtual void render(UIRenderer *renderer, const color_t &color);
	
	virtual void setLocation(const aabox2f &rect);
	virtual void init(const TreeDataObject *object);
	virtual vector2f calculateMinimumSize() const;
	
	void setPicture(size_t index, const PicturePtr &picture);
	const PicturePtr & getPicture(size_t index) const;
	
	void setWidthLeft(f32 width);
	f32 getWidthLeft() const;
	
	void setWidthRight(f32 width);
	f32 getWidthRight() const;
	
	void setHeightTop(f32 height);
	f32 getHeightTop() const;
	
	void setHeightBottom(f32 height);
	f32 getHeightBottom() const;
	
protected:
	aabox2f m_rect[9];
	PicturePtr m_picture[9];
	f32 m_widthLeft, m_widthRight;
	f32 m_heightTop, m_heightBottom;
};

_vn_end

#endif
