//
//  vnUILabel.h
//  vn2d
//
//  Created by Wayne on 14-1-14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUILabel_h
#define vn2d_vnUILabel_h

#include "vnUIElement.h"
#include "vnFontString.h"

_vn_begin

class _vn_2d_api UILabel : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    
    // ctor & dtor
    UILabel();
    virtual UILabel * clone() const;
    
    
    // properties.
    
    u32 getTextAlignment() const;
    void setTextAlignment(u32 alignment);
    
    void setFont(const FontPtr &font);
    const FontPtr & getFont() const;
    
    void setText(const str8 &text);
    void setText(const str32 &text);
    
    void setTextColor(const color_t &color);
    const color_t & getTextColor() const;
	//add by ts
	void setFontStroke(bool enabled, color_t color = 0xFFFFFFFF);
    
    // methods
    virtual void init(const TreeDataObject *object);
    
    FontString & _string();
    const FontString & _string() const;
    
protected:
    virtual void _onUpdate(f32 deltaTime);
	virtual void _onRender(UIRenderer *renderer);
    
    FontPtr m_font;
    FontString m_string;
    u32 m_textAlignment;
    color_t m_textColor;
    vector2f m_textPosition;
	//add by ts
	bool m_strokeEnabled;
	color_t m_strokeColor;
    
};

_vn_end

#endif
