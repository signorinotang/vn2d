//
//  vnPictureManager.h
//  vngfx
//
//  Created by Wayne on 13-12-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnPictureManager_h
#define vngfx_vnPictureManager_h

#include "vnSingleton.h"
#include "vnPicture.h"
#include "vnFilePath.h"

#include <map>
#include <vector>
#include <list>

_vn_begin

class _vn_gfx_api PictureManager : public Singleton<PictureManager> {
public:
    PictureManager();
    ~PictureManager();
    
    Picture * createPicture(const c8 *name);
    Picture * createPicture(const FilePath &_file, const str8 &_name);
	bool list(const FilePath &file, std::list<str8> &result);

    void setScale(const vector2f &scale);
    const vector2f & getScale() const;
private:
    struct PictureInfo {
        FilePath *texture;
        aabox2i rect;
    };
    struct PictureSetInfo : AllocatedObject {
        typedef std::map<str8, PictureInfo> PictureMap;
        PictureMap pictures;
        typedef std::vector<FilePath> Textures;
        Textures textures;
    };
    typedef std::map<FilePath, PictureSetInfo *> PictureSetInfoMap;
    PictureSetInfoMap m_info;
    
    vector2f m_scale;
};

_vn_end

#endif
