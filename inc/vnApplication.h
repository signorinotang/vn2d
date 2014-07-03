//
//  vnApplication.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnApplication_h
#define vn2d_vnApplication_h

#include "vnSingleton.h"
#include "vnVector2.h"
#include "vnLink.h"

_vn_begin

class Device;
class ApplicationImpl;
class _vn_2d_api Application : public Singleton<Application> {
public:
    enum Type {
        kType_iPhone_3_5 = 1,
        kType_iPhone_4,
        kType_iPad,
    };
    
    class Delegate {
    public:
        virtual void onInitialize() {}
        virtual void onUpdate(f32 deltaTime) {}
        virtual void onRender(Device *device) {}
        virtual void onFinalize() {}
        virtual void onSuspend() {}
        virtual void onResume() {}
        virtual void onTouchBegan(long tId, const vector2f &pos) {}
		virtual void onTouchMoved(long tId, const vector2f &pos) {}
		virtual void onTouchEnded(long tId, const vector2f &pos) {}
		virtual void onTouchCancelled(long tId, const vector2f &pos) {}
		virtual void onKeyboardWillShow() {}
		virtual void onKeyboardDidShow() {}
		virtual void onKeyboardWillHide() {}
		virtual void onKeyboardDidHide() {}
		virtual void onInsertText(const str32 &text) {}
		virtual void onDeleteBackward() {}
		virtual void onMarkText(const str32 &text) {}
        virtual void onUnmarkText() {}
    };
    
    
    Application();
    ~Application();
    
    static int main(Delegate *delegate, bool horizontal);
    
    Type type();
    f32 scale();
    const vector2i & viewSize();
    
    const str8 & resourcePath();
    const str8 & cachePath();
    const str8 & tempPath();
    const str8 & documentPath();
    
    void requireInputFocus();
    bool hasInputFocus();
    void releaseInputFocus();
    void setInputAsciiOnly(bool flag);
    bool isInputAsciiOnly();
    
    Device * createDevice(const vector2f &size);
    
    Link * createLink(LinkHandler *handler);
    
private:
    ApplicationImpl *m_impl;
};

_vn_end

#endif
