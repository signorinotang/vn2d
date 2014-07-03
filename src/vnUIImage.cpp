//
//  vnUIImage.cpp
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUIImage.h"

#include "vnUIRenderer.h"
#include "vnPictureManager.h"

_vn_begin

void UIImage::update(f32 deltaTime) {
	
}

void UIImage::render(UIRenderer *renderer, const color_t &color) {
	
}

void UIImage::setLocation(const aabox2f &rect) {
	
}

void UIImage::init(const TreeDataObject *object) {
	
}

vector2f UIImage::calculateMinimumSize() const {
	return vector2f::kZero;
}

UIImageEmpty * UIImageEmpty::clone() const {
	return vnnew UIImageEmpty();
}

UIImage0 * UIImage0::clone() const {
	return vnnew UIImage0(*this);
}

void UIImage0::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect, color);
}

void UIImage0::setLocation(const aabox2f &rect) {
	m_rect = rect;
}

UIImage1 * UIImage1::clone() const {
	return vnnew UIImage1(*this);
}

void UIImage1::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect, color, m_picture.ptr());
}

void UIImage1::init(const vn::TreeDataObject *object) {
	const TreeDataString *tdstr = object->queryAttributeString("image");
    if (tdstr) {
        m_picture = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
}

void UIImage1::setPicture(const PicturePtr &picture) {
	m_picture = picture;
}

const PicturePtr & UIImage1::getPicture() const {
	return m_picture;
}

UIImage3V::UIImage3V()
: m_heightTop(0)
, m_heightBottom(0) {
	
}

UIImage3V * UIImage3V::clone() const {
	return vnnew UIImage3V(*this);
}

void UIImage3V::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect[0], color, m_picture[0].ptr());
	renderer->drawRect(m_rect[1], color, m_picture[1].ptr());
	renderer->drawRect(m_rect[2], color, m_picture[2].ptr());
}

void UIImage3V::setLocation(const aabox2f &rect) {
	m_rect[0].min_corner = rect.min_corner;
	m_rect[0].max_corner.set(rect.max_corner.x, rect.min_corner.y + m_heightTop);
	m_rect[2].min_corner.set(rect.min_corner.x, rect.max_corner.y - m_heightBottom);
	m_rect[2].max_corner = rect.max_corner;
	m_rect[1].min_corner.set(rect.min_corner.x, m_rect[0].max_corner.y);
	m_rect[1].max_corner.set(rect.max_corner.x, m_rect[2].min_corner.y);
}

void UIImage3V::init(const TreeDataObject *object) {
	const TreeDataString *tdstr = object->queryAttributeString("image1");
    if (tdstr) {
        m_picture[0] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image2");
    if (tdstr) {
        m_picture[1] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image3");
    if (tdstr) {
        m_picture[2] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	
	m_heightTop = object->queryAttributeFloat("height_top");
	m_heightBottom = object->queryAttributeFloat("height_bottom");
}

vector2f UIImage3V::calculateMinimumSize() const {
	return vector2f(0, m_heightTop + m_heightBottom);
}

void UIImage3V::setPicture(size_t index, const PicturePtr &picture) {
	vnassert(index < 3);
	m_picture[index] = picture;
}

const PicturePtr & UIImage3V::getPicture(size_t index) const {
	vnassert(index < 3);
	return m_picture[index];
}

void UIImage3V::setHeightTop(f32 height) {
	m_heightTop = height;
}

f32 UIImage3V::getHeightTop() const {
	return m_heightTop;
}

void UIImage3V::setHeightBottom(f32 height) {
	m_heightBottom = height;
}

f32 UIImage3V::getHeightBottom() const {
	return m_heightBottom;
}

UIImage3H::UIImage3H()
: m_widthLeft(0)
, m_widthRight(0) {
	
}

UIImage3H * UIImage3H::clone() const {
	return vnnew UIImage3H(*this);
}

void UIImage3H::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect[0], color, m_picture[0].ptr());
	renderer->drawRect(m_rect[1], color, m_picture[1].ptr());
	renderer->drawRect(m_rect[2], color, m_picture[2].ptr());
}

void UIImage3H::setLocation(const aabox2f &rect) {
	m_rect[0].min_corner = rect.min_corner;
	m_rect[0].max_corner.set(rect.min_corner.x + m_widthLeft, rect.max_corner.y);
	m_rect[2].min_corner.set(rect.max_corner.x - m_widthRight, rect.min_corner.y);
	m_rect[2].max_corner = rect.max_corner;
	m_rect[1].min_corner.set(m_rect[0].max_corner.x, rect.min_corner.y);
	m_rect[1].max_corner.set(m_rect[2].min_corner.x, rect.max_corner.y);
}

void UIImage3H::init(const TreeDataObject *object) {
	const TreeDataString *tdstr = object->queryAttributeString("image1");
    if (tdstr) {
        m_picture[0] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image2");
    if (tdstr) {
        m_picture[1] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image3");
    if (tdstr) {
        m_picture[2] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	
	m_widthLeft = object->queryAttributeFloat("width_left");
	m_widthRight = object->queryAttributeFloat("width_right");
}

vector2f UIImage3H::calculateMinimumSize() const {
	return vector2f(m_widthLeft + m_widthRight, 0);
}

void UIImage3H::setPicture(size_t index, const PicturePtr &picture) {
	vnassert(index < 3);
	m_picture[index] = picture;
}

const PicturePtr & UIImage3H::getPicture(size_t index) const {
	vnassert(index < 3);
	return m_picture[index];
}

void UIImage3H::setWidthLeft(f32 width) {
	m_widthLeft = width;
}

f32 UIImage3H::getWidthLeft() const {
	return m_widthLeft;
}

void UIImage3H::setWidthRight(f32 width) {
	m_widthRight = width;
}

f32 UIImage3H::getWidthRight() const {
	return m_widthRight;
}

UIImage9::UIImage9()
: m_widthLeft(0)
, m_widthRight(0)
, m_heightTop(0)
, m_heightBottom(0) {
	
}

UIImage9 * UIImage9::clone() const {
	return vnnew UIImage9(*this);
}

void UIImage9::render(UIRenderer *renderer, const color_t &color) {
	renderer->drawRect(m_rect[0], color, m_picture[0].ptr());
	renderer->drawRect(m_rect[1], color, m_picture[1].ptr());
	renderer->drawRect(m_rect[2], color, m_picture[2].ptr());
	renderer->drawRect(m_rect[3], color, m_picture[3].ptr());
	renderer->drawRect(m_rect[4], color, m_picture[4].ptr());
	renderer->drawRect(m_rect[5], color, m_picture[5].ptr());
	renderer->drawRect(m_rect[6], color, m_picture[6].ptr());
	renderer->drawRect(m_rect[7], color, m_picture[7].ptr());
	renderer->drawRect(m_rect[8], color, m_picture[8].ptr());
}

void UIImage9::setLocation(const aabox2f &rect) {
	m_rect[4].min_corner = rect.min_corner + vector2f(m_widthLeft, m_heightTop);
	m_rect[4].max_corner = rect.max_corner - vector2f(m_widthRight, m_heightBottom);
	
	m_rect[0].min_corner = rect.min_corner;
	m_rect[0].max_corner = m_rect[4].min_corner;
	
	m_rect[1].min_corner.set(m_rect[4].min_corner.x, rect.min_corner.y);
	m_rect[1].max_corner.set(m_rect[4].max_corner.x, m_rect[4].min_corner.y);
	
	m_rect[2].min_corner.set(m_rect[4].max_corner.x, rect.min_corner.y);
	m_rect[2].max_corner.set(rect.max_corner.x, m_rect[4].min_corner.y);
	
	m_rect[3].min_corner.set(rect.min_corner.x, m_rect[4].min_corner.y);
	m_rect[3].max_corner.set(m_rect[4].min_corner.x, m_rect[4].max_corner.y);
	
	m_rect[5].min_corner.set(m_rect[4].max_corner.x, m_rect[4].min_corner.y);
	m_rect[5].max_corner.set(rect.max_corner.x, m_rect[4].max_corner.y);
	
	m_rect[6].min_corner.set(rect.min_corner.x, m_rect[4].max_corner.y);
	m_rect[6].max_corner.set(m_rect[4].min_corner.x, rect.max_corner.y);
	
	m_rect[7].min_corner.set(m_rect[4].min_corner.x, m_rect[4].max_corner.y);
	m_rect[7].max_corner.set(m_rect[4].max_corner.x, rect.max_corner.y);
	
	m_rect[8].min_corner = m_rect[4].max_corner;
	m_rect[8].max_corner = rect.max_corner;
}

void UIImage9::init(const TreeDataObject *object) {
	const TreeDataString *tdstr = object->queryAttributeString("image11");
    if (tdstr) {
        m_picture[0] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image12");
    if (tdstr) {
        m_picture[1] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image13");
    if (tdstr) {
        m_picture[2] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image21");
    if (tdstr) {
        m_picture[3] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image22");
    if (tdstr) {
        m_picture[4] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image23");
    if (tdstr) {
        m_picture[5] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image31");
    if (tdstr) {
        m_picture[6] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image32");
    if (tdstr) {
        m_picture[7] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	tdstr = object->queryAttributeString("image33");
    if (tdstr) {
        m_picture[8] = PictureManager::instance().createPicture(tdstr->value().c_str());
    }
	
	m_widthLeft = object->queryAttributeFloat("width_left");
	m_widthRight = object->queryAttributeFloat("width_right");
	m_heightTop = object->queryAttributeFloat("height_top");
	m_heightBottom = object->queryAttributeFloat("height_bottom");
}

vector2f UIImage9::calculateMinimumSize() const {
	return vector2f(m_widthLeft + m_widthRight, m_heightTop + m_heightBottom);
}

void UIImage9::setPicture(size_t index, const PicturePtr &picture) {
	vnassert(index < 9);
	m_picture[index] = picture;
}

const PicturePtr & UIImage9::getPicture(size_t index) const {
	vnassert(index < 9);
	return m_picture[index];
}

void UIImage9::setWidthLeft(f32 width) {
	m_widthLeft = width;
}

f32 UIImage9::getWidthLeft() const {
	return m_widthRight;
}

void UIImage9::setWidthRight(f32 width) {
	m_widthRight = width;
}

f32 UIImage9::getWidthRight() const {
	return m_widthRight;
}

void UIImage9::setHeightTop(f32 height) {
	m_heightTop = height;
}

f32 UIImage9::getHeightTop() const {
	return m_heightTop;
}

void UIImage9::setHeightBottom(f32 height) {
	m_heightBottom = height;
}

f32 UIImage9::getHeightBottom() const {
	return m_heightBottom;
}

_vn_end