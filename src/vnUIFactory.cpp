//
//  vnUIFactory.cpp
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUIFactory.h"
#include "vnFileManager.h"
#include "vnTreeDataParser.h"
#include "vnLog.h"

#include "vnUIFrame.h"
#include "vnUIPicture.h"
#include "vnUILabel.h"
#include "vnUIButton.h"
#include "vnUITextField.h"

#include "vnUILocatorRect.h"
#include "vnUILocatorBorder.h"
#include "vnUILocatorBox.h"

#include "vnUIImage.h"

_vn_begin

VN_IMPL_UI_ELEMENT(UIElement, "Element")
VN_IMPL_UI_ELEMENT(UIFrame, "Frame")
VN_IMPL_UI_ELEMENT(UIPicture, "Picture")
VN_IMPL_UI_ELEMENT(UILabel, "Label")
VN_IMPL_UI_ELEMENT(UIButton, "Button")
VN_IMPL_UI_ELEMENT(UITextField, "TextField")

VN_IMPL_UI_LOCATOR(UILocatorRect, "Rect")
VN_IMPL_UI_LOCATOR(UILocatorRectEx, "RectEx")
VN_IMPL_UI_LOCATOR(UILocatorBorder, "Border")
VN_IMPL_UI_LOCATOR(UILocatorBox, "Box")
VN_IMPL_UI_LOCATOR(UILocatorBoxEx, "BoxEx")

VN_IMPL_UI_IMAGE(UIImage0, "Image0")
VN_IMPL_UI_IMAGE(UIImage1, "Image1")
VN_IMPL_UI_IMAGE(UIImage3H, "Image3H")
VN_IMPL_UI_IMAGE(UIImage3V, "Image3V")
VN_IMPL_UI_IMAGE(UIImage9, "Image9")

void UIFactory::_register(const c8 *name, UIElement*(*creator)()) {
    m_factoryE[name] = creator;
}

void UIFactory::_register(const c8 *name, UILocator *(*creator)(UIElement *)) {
    m_factoryL[name] = creator;
}

void UIFactory::_register(const c8 *name, UIImage *(*creator)()) {
	m_factoryI[name] = creator;
}


UIElement * UIFactory::createElement(const str8 &name) {
    ElementFactory::iterator it = m_factoryE.find(name);
    if (it == m_factoryE.end()) {
        return 0;
    }
    return it->second();
}

UIElement * UIFactory::createElement(const TreeDataObject *object) {
    vnassert(object);
    UIElement *p = createElement(object->name());
    if (!p) {
        VN_LOG_ERROR("UIFactory: unknown element \"" << object->name() << "\"");
        return 0;
    }
    p->init(object);
    return p;
}

UIElement * UIFactory::createElement(const TreeDataNode *node) {
    vnassert(node);
    UIElement *p = createElement((const TreeDataObject *)node);
    if (!p) {
        return 0;
    }
    createElements(p, node->firstChild());
    return p;
}

UILocator * UIFactory::createLocator(const str8 &name, UIElement *owner) {
    vnassert(owner != 0);
    LocatorFactory::iterator it = m_factoryL.find(name);
    if (it == m_factoryL.end()) {
        return 0;
    }
    return it->second(owner);
}

UIImage * UIFactory::createImage(const str8 &name) {
	ImageFactory::iterator it = m_factoryI.find(name);
	if (it == m_factoryI.end()) {
		return 0;
	}
	return it->second();
}

UIImage * UIFactory::createImage(const TreeDataObject *object) {
	vnassert(object);
	UIImage *img = createImage(object->name());
	if (!img) {
		VN_LOG_ERROR("UIFactory: unknown image \"" << object->name() << "\"");
		return 0;
	}
	img->init(object);
	return img;
}

void UIFactory::createElements(UIElement *root, const TreeDataNode *node) {
    vnassert(root);
    while (node) {
        UIElement *p = createElement((TreeDataObject *)node);
        if (p) {
            root->addChild(p, false);
            createElements(p, node->firstChild());
        }
        node = node->nextNeighbour();
    }
}

bool UIFactory::createElementsFromBinaryTDL(UIElement *root, const FilePath &path) {
    FileStream *fs = FileManager::instance().open(path);
    if (!fs) {
        VN_LOG_ERROR("UIFactory: open file \"" << path.fsId << ":/" << path.fileName << "\" failed.");
        return false;
    }
    TreeData *node = 0;
    if (!TreeData::load(*fs, node) || node->type() != TreeData::kNode) {
        fs->close();
        VN_LOG_ERROR("UIFactory: read file \"" << path.fsId << ":/" << path.fileName << "\" failed.");
        return false;
    }
    fs->close();
    createElements(root, (TreeDataNode *)node);
    return true;
}

bool UIFactory::createElementsFromTDL(vn::UIElement *root, const vn::FilePath &path) {
    TreeDataNode *node = TreeDataParser::instance().parse(path);
    if (!node) {
        return false;
    }
    createElements(root, node);
    node->drop();
    return true;
}

_vn_end
