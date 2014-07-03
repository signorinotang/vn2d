//
//  vnUIRoot.h
//  vn2d
//
//  Created by Wayne on 14-1-14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUIRoot_h
#define vn2d_vnUIRoot_h

#include "vnSingleton.h"
#include "vnUICommon.h"
#include "vnUIRenderer.h"

#include <map>

_vn_begin

class UIElement;
class UIRenderer;

class _vn_2d_api UIRoot : public Singleton<UIRoot> {
public:
    class TouchFilter {
    public:
        virtual bool filter_onTouchBegan(const UITouch *touch) { return false; }
        virtual bool filter_onTouchMoved(const UITouch *touch) { return false; }
        virtual bool filter_onTouchEnded(const UITouch *touch) { return false; }
        virtual bool filter_onTouchCancelled(const UITouch *touch) { return false; }
    };
    
	UIRoot();
	~UIRoot();
    
	void update(f32 deltaTime);
	void render();
	void setRootElement(UIElement *root, bool grab = true);
	bool dispatchTouchBegan(long touchId, const vector2f &position);
    bool dispatchTouchMoved(long touchId, const vector2f &position);
    bool dispatchTouchEnded(long touchId, const vector2f &position);
    bool dispatchTouchCancelled(long touchId, const vector2f &position);
    
	void dispatchInsertText(const str32 &text);
	void dispatchDeleteBackward();
	void dispatchMarkText(const str32 &text);
    void dispatchUnmarkText();
    
	void reset();
	bool captureTouch(UIElement *element, long touchId);
	void releaseTouch(UIElement *element, long touchId);
	void requireFocus(UIElement *element);
	void releaseFocus(UIElement *element);
    
    void addTouchFilter(TouchFilter *filter);
    void removeTouchFilter(TouchFilter *filter);
	
    void setViewBox(const aabox2f &rect);
	const aabox2f & getViewBox() const;
private:
	UIRenderer m_renderer;
	UIElement *m_root;
	UIElement *m_focus;
	struct TouchData {
		UITouch touch;
		UIElement *hover;
		UIElement *capture;
		TouchData();
		~TouchData();
	};
	typedef std::map<long, TouchData> TouchDataMap;
	TouchDataMap m_touches;
	aabox2f m_viewBox;
	
    typedef std::list<TouchFilter *> TouchFilters;
    TouchFilters m_touchFilters;
    
	TouchData & _newTouch(long touchId, const vector2f &position);
	bool _updateTouch(long touchId, const vector2f &position, TouchData *&touch);
    
	void _updateTouches();
	void _updateInput();
	
	void _resetTouches();
	void _resetFocus();
};

_vn_end

#endif
