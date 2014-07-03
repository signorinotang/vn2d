//
//  vnUIRenderEffect.h
//  vn2d
//
//  Created by Wayne on 3/20/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUIRenderEffect_h
#define vn2d_vnUIRenderEffect_h

#include "vnRefCounted.h"
#include "vnColor.h"
#include "vnMatrix3.h"

_vn_begin

class UIRenderer;
class UIElement;

class _vn_2d_api UIRenderEffect : public RefCounted {
public:
    UIRenderEffect();
    
    virtual void update(UIElement *element, f32 deltaTime) = 0;
    virtual void begin(UIRenderer *renderer, UIElement *element) = 0;
    virtual void end(UIRenderer *renderer, UIElement *element) = 0;
    
    bool valid() const;
    
protected:
    bool m_valid;
};

class _vn_2d_api UIRenderEffect_Color : virtual public UIRenderEffect {
public:
    UIRenderEffect_Color();
    
    virtual void update(UIElement *element, f32 deltaTime);
    virtual void begin(UIRenderer *renderer, UIElement *element);
    virtual void end(UIRenderer *renderer, UIElement *element);
    
    color_t & color();
    const color_t & color() const;
protected:
    color_t m_color;
};

class _vn_2d_api UIRenderEffect_Transform : virtual public UIRenderEffect {
public:
    UIRenderEffect_Transform();
    
    virtual void update(UIElement *element, f32 deltaTime);
    virtual void begin(UIRenderer *renderer, UIElement *element);
    virtual void end(UIRenderer *renderer, UIElement *element);
    
    matrix3 & matrix();
    const matrix3 & matrix() const;
    
protected:
    matrix3 m_matrix;
};

class _vn_2d_api UIRenderEffect_TransformAndColor : public UIRenderEffect_Transform, public UIRenderEffect_Color {
public:
    virtual void update(UIElement *element, f32 deltaTime);
    virtual void begin(UIRenderer *renderer, UIElement *element);
    virtual void end(UIRenderer *renderer, UIElement *element);
};

class _vn_2d_api UIRenderEffect_Shake : public UIRenderEffect_Transform {
public:
    UIRenderEffect_Shake(f32 range = 1.f);
    
    virtual void update(UIElement *element, f32 deltaTime);
    
protected:
    f32 m_range;
    s32 m_count;
};

_vn_end

#endif
