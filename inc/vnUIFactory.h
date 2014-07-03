//
//  vnUIFactory.h
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUIFactory_h
#define vn2d_vnUIFactory_h

#include "vnSingleton.h"
#include "vnTreeData.h"
#include "vnFilePath.h"
#include "vnUICommon.h"

#include <map>

_vn_begin

class UIElement;
class UILocator;
class UIImage;


class _vn_2d_api UIFactory : public Singleton<UIFactory> {
public:
    typedef std::map<str8, UIElement *> ElementMap;
    UIElement * createElement(const str8 &name);
    UIElement * createElement(const TreeDataObject *object);
    UIElement * createElement(const TreeDataNode *node);
	UILocator * createLocator(const str8 &name, UIElement *owner);
	UIImage * createImage(const str8 &name);
	UIImage * createImage(const TreeDataObject *object);
    
    void createElements(UIElement *root, const TreeDataNode *node);
    bool createElementsFromTDL(UIElement *root, const FilePath &path);
    bool createElementsFromBinaryTDL(UIElement *root, const FilePath &path);
    
    void _register(const c8 *name, UIElement *(*creator)());
	void _register(const c8 *name, UILocator *(*creator)(UIElement *));
	void _register(const c8 *name, UIImage *(*creator)());
    
protected:
    typedef std::map<str8, UIElement *(*)()> ElementFactory;
	typedef std::map<str8, UILocator *(*)(UIElement *)> LocatorFactory;
	typedef std::map<str8, UIImage *(*)()> ImageFactory;
    
    ElementFactory m_factoryE;
	LocatorFactory m_factoryL;
	ImageFactory m_factoryI;
};

_vn_end

#define VN_DECL_UI_ELEMENT() \
public:\
static _vn_ns::UIElement * create();

#define VN_IMPL_UI_ELEMENT(cls, name) \
struct _auto_reg_##cls {\
_auto_reg_##cls() {\
_vn_ns::UIFactory::instance()._register(name, cls::create);\
}\
} _auto_reg_##cls##_ins;\
_vn_ns::UIElement * cls::create() { return vnnew cls(); }

#define VN_DECL_UI_LOCATOR() \
public:\
static _vn_ns::UILocator * create(_vn_ns::UIElement *owner);

#define VN_IMPL_UI_LOCATOR(cls, name) \
struct _auto_reg_##cls {\
_auto_reg_##cls() {\
_vn_ns::UIFactory::instance()._register(name, cls::create);\
}\
} _auto_reg_##cls##_ins;\
_vn_ns::UILocator * cls::create(_vn_ns::UIElement *owner) { return vnnew cls(owner); }

#define VN_DECL_UI_IMAGE() \
public:\
static _vn_ns::UIImage * create();

#define VN_IMPL_UI_IMAGE(cls, name) \
struct _auto_reg_##cls {\
_auto_reg_##cls() {\
_vn_ns::UIFactory::instance()._register(name, cls::create);\
}\
} _auto_reg_##cls##_ins;\
_vn_ns::UIImage * cls::create() { return vnnew cls(); }




#endif
