//
//  vnUIPicture.cpp
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIPicture.h"
#include "vnUIRenderer.h"
#include "vnPictureManager.h"

_vn_begin

UIPicture::UIPicture()
: m_color(color_t::kWhite)
, m_image(vnnew UIImage0()){
    
}

UIPicture::UIPicture(const UIPicture &other)
: UIElement(other)
, m_color(other.m_color)
, m_image(other.m_image->clone()) {
	
}

UIPicture * UIPicture::clone() const {
    return vnnew UIPicture(*this);
}

const color_t & UIPicture::getColor() const {
    return m_color;
}

void UIPicture::setColor(const color_t &color) {
    m_color = color;
}

const UIImagePtr & UIPicture::getImage() const {
    return m_image;
}

void UIPicture::setImage(const UIImagePtr &image) {
	vnassert(!image.null());
    m_image = image;
    m_image->setLocation(m_boundingBox);
}

void UIPicture::init(const TreeDataObject *object) {
    UIElement::init(object);
    m_color.convertFromBGRA(object->queryAttributeInteger("color", 0xFFFFFFFF));
	const TreeDataObject *tdobj = object->queryAttributeObject("image");
	UIImage *img = 0;
	if (tdobj) {
		img = UIFactory::instance().createImage(tdobj);
	}
	if (img) {
		m_image = img;
	}
}

void UIPicture::_onBoundingBoxUpdated() {
	UIElement::_onBoundingBoxUpdated();
	m_image->setLocation(m_boundingBox);
}

void UIPicture::_onUpdate(f32 deltaTime) {
	m_image->update(deltaTime);
}

void UIPicture::_onRender(UIRenderer *renderer) {
    m_image->render(renderer, m_color);
}

_vn_end
