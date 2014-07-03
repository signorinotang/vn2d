//
//  vnUIRenderEffect.cpp
//  vn2d
//
//  Created by Wayne on 3/20/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUIRenderEffect.h"
#include "vnUIRenderer.h"

_vn_begin

UIRenderEffect::UIRenderEffect()
: m_valid(true) {
    
}

bool UIRenderEffect::valid() const {
    return m_valid;
}

UIRenderEffect_Color::UIRenderEffect_Color()
: m_color(color_t::kWhite) {
    
}

void UIRenderEffect_Color::update(UIElement *element, f32 deltaTime) {
    
}

void UIRenderEffect_Color::begin(UIRenderer *renderer, UIElement *element) {
    renderer->pushBlendColor(m_color);
}

void UIRenderEffect_Color::end(UIRenderer *renderer, UIElement *element) {
    renderer->popBlendColor();
}

color_t & UIRenderEffect_Color::color() {
    return m_color;
}

const color_t & UIRenderEffect_Color::color() const {
    return m_color;
}

UIRenderEffect_Transform::UIRenderEffect_Transform()
: m_matrix(matrix3::c_identity) {
    
}

void UIRenderEffect_Transform::update(UIElement *element, f32 deltaTime) {
    
}

void UIRenderEffect_Transform::begin(UIRenderer *renderer, UIElement *element) {
    renderer->pushTransform(m_matrix);
}

void UIRenderEffect_Transform::end(UIRenderer *renderer, UIElement *element) {
    renderer->popTransform();
}
    
matrix3 & UIRenderEffect_Transform::matrix() {
    return m_matrix;
}

const matrix3 & UIRenderEffect_Transform::matrix() const {
    return m_matrix;
}

void UIRenderEffect_TransformAndColor::update(UIElement *element, f32 deltaTime) {
    
}

void UIRenderEffect_TransformAndColor::begin(UIRenderer *renderer, UIElement *element) {
    renderer->pushTransform(m_matrix);
    renderer->pushBlendColor(m_color);
}

void UIRenderEffect_TransformAndColor::end(UIRenderer *renderer, UIElement *element) {
    renderer->popTransform();
    renderer->popBlendColor();
}

UIRenderEffect_Shake::UIRenderEffect_Shake(f32 range)
: m_count(0)
, m_range(range) {
}

void UIRenderEffect_Shake::update(UIElement *element, f32 deltaTime) {
    m_count = (m_count + 1) % 16;
    switch (m_count) {
        case 0:
            m_matrix.m31 = -m_range;
            m_matrix.m32 = -m_range;
            break;
            
        case 1:
            m_matrix.m31 = m_range;
            m_matrix.m32 = m_range;
            break;
            
        case 2:
            m_matrix.m31 = 0;
            m_matrix.m32 = -m_range;
            break;
            
        case 3:
            m_matrix.m31 = 0;
            m_matrix.m32 = m_range;
            break;
            
        case 4:
            m_matrix.m31 = m_range;
            m_matrix.m32 = -m_range;
            break;
            
        case 5:
            m_matrix.m31 = -m_range;
            m_matrix.m32 = m_range;
            break;
            
        case 6:
            m_matrix.m31 = m_range;
            m_matrix.m32 = 0;
            break;
            
        case 7:
            m_matrix.m31 = -m_range;
            m_matrix.m32 = 0;
            break;
            
        case 8:
            m_matrix.m31 = m_range;
            m_matrix.m32 = m_range;
            break;
            
        case 9:
            m_matrix.m31 = -m_range;
            m_matrix.m32 = -m_range;
            break;
            
        case 10:
            m_matrix.m31 = 0;
            m_matrix.m32 = m_range;
            break;
            
        case 11:
            m_matrix.m31 = 0;
            m_matrix.m32 = -m_range;
            break;
            
        case 12:
            m_matrix.m31 = -m_range;
            m_matrix.m32 = m_range;
            break;
            
        case 13:
            m_matrix.m31 = m_range;
            m_matrix.m32 = -m_range;
            break;
            
        case 14:
            m_matrix.m31 = -m_range;
            m_matrix.m32 = 0;
            break;
            
        case 15:
            m_matrix.m31 = m_range;
            m_matrix.m32 = 0;
            break;
    }
}

_vn_end