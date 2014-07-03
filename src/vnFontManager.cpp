//
//  vnFontManager.cpp
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnFontManager.h"
#include "vnTreeDataParser.h"
#include "vnDevice.h"

_vn_begin

void FontManager::setFont(const str8 &name, const FontPtr &font) {
    if (font.null()) {
        m_fonts.erase(name);
    } else {
        m_fonts[name] = font;
    }
}

const FontPtr & FontManager::getFont(const str8 &name) {
    Fonts::iterator it = m_fonts.find(name);
    if (it == m_fonts.end()) {
        return m_default;
    }
    return it->second;
}

void FontManager::setDefaultFont(const FontPtr &font) {
    m_default = font;
}

const FontPtr & FontManager::getDefaultFont() const {
    return m_default;
}

void FontManager::discardAll() {
    if (!m_default.null()) {
        m_default->discard();
    }
    for (Fonts::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it) {
        it->second->discard();
    }
}

bool FontManager::loadFromTDL(const vn::FilePath &file) {
    TreeDataNode *root = TreeDataParser::instance().parse(file);
    if (!root) {
        return false;
    }
    Device &device = Device::instance();
    s32 default_size = root->queryAttributeInteger("default_font_size", 16);
    if (default_size) {
        m_default = device.createFont(default_size);
    }
    for (TreeDataNode *p = root->firstChild(); p; p = p->nextNeighbour()) {
        const TreeDataString *tds = p->queryAttributeString("name");
        if (!tds) {
            continue;
        }
        if (p->name() == "font") {
            s32 size = p->queryAttributeInteger("size");
            if (size) {
                setFont(tds->value(), device.createFont(size));
            }
        } else {
            Factory::iterator it = m_factory.find(p->name());
            if (it != m_factory.end()) {
                Font *font = it->second(p);
                if (font) {
                    setFont(tds->value(), font);
                }
            }
        }
    }
    root->drop();
    return true;
}

void FontManager::registerFontType(const str8 &name, Font *(*creator)(const TreeDataNode *)) {
    vnassert(creator);
    m_factory[name] = creator;
}

_vn_end