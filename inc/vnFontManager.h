//
//  vnFontManager.h
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnFontManager_h
#define vn2d_vnFontManager_h

#include "vnSingleton.h"
#include "vnFont.h"
#include "vnFilePath.h"
#include "vnTreeData.h"
#include <map>

_vn_begin

class _vn_2d_api FontManager : public Singleton<FontManager> {
public:
    void setFont(const str8 &name, const FontPtr &font);
    const FontPtr & getFont(const str8 &name);
    
    void setDefaultFont(const FontPtr &font);
    const FontPtr & getDefaultFont() const;
    void discardAll();
    
    bool loadFromTDL(const FilePath &file);
    
    void registerFontType(const str8 &name, Font *(*creator)(const TreeDataNode *));
protected:
    typedef std::map<str8, FontPtr> Fonts;
    Fonts m_fonts;
    FontPtr m_default;
    
    typedef std::map<str8, Font *(*)(const TreeDataNode *)> Factory;
    Factory m_factory;
};


_vn_end


#endif
