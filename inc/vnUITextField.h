//
//  vnUITextField.h
//  vn2d
//
//  Created by Wayne on 2/24/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUITextField_h
#define vn2d_vnUITextField_h

#include "vnUIElement.h"
#include "vnFontString.h"

_vn_begin

class _vn_2d_api UITextField : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    UITextField();
    virtual UITextField * clone() const;
    
    
    u32 getTextAlignment() const;
    void setTextAlignment(u32 alignment);
    
    void setFont(const FontPtr &font);
    const FontPtr & getFont() const;
    
    void setText(const str8 &text);
    void setText(const str32 &text);
    const str32 & getText() const;
    
    void setTextColor(const color_t &color);
    const color_t & getTextColor() const;
    
    void setMarkedColor(const color_t &color);
    const color_t & getMarkedColor() const;
    
    void setASCIIOnly(bool asciiOnly);
    bool isASCIIOnly() const;
    
    // methods
    virtual void init(const TreeDataObject *object);
    virtual KeyboardType keyboardType() const;
    
    // events
	virtual bool onTouchBegan(UIElement *target, const UITouch *touch);
	
	virtual void onInsertText(const str32 &text);
	virtual void onDeleteBackward();
	virtual void onMarkText(const str32 &text);
    virtual void onUnmarkText();
    
	virtual void onFocusIn();
	virtual void onFocusOut();
    
    
    // named "newline"
    void bindAction_NewLine(const function<void(UIElement *, const str32 &)> &func);
    void bindAction_FocusIn(const function<void(UIElement *)> &func);
    void bindAction_FocusOut(const function<void(UIElement *)> &func);
    
protected:
    virtual void _onUpdate(f32 deltaTime);
	virtual void _onRender(UIRenderer *renderer);
    virtual bool _bindAction(const c8 *name, RefCounted *func_impl);
    
    virtual void _onNewLine();
    
    FontPtr m_font;
    FontString m_text;
    FontString m_markedText;
    u32 m_textAlignment;
    color_t m_textColor;
    color_t m_markedColor;
    vector2f m_textPosition;
    vector2f m_markedTextPosition;
    aabox2f m_markedRect;
    aabox2f m_caretRect;
    bool m_hasFocus;
    bool m_asciiOnly;
    f32 m_caretTimer;
    
    function<void(UIElement *, const str32 &)> m_fnNewLine;
    function<void(UIElement *)> m_fnFocusIn;
    function<void(UIElement *)> m_fnFocusOut;
};

_vn_end

#endif
