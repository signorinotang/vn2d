//
//  vnTextureManager.h
//  vngfx
//
//  Created by Wayne on 13-12-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnTextureManager_h
#define vngfx_vnTextureManager_h

#include "vnTexture.h"
#include "vnSingleton.h"
#include "vnFilePath.h"

#include <map>

_vn_begin

class FileStream;

class _vn_gfx_api TextureManager : public Singleton<TextureManager> {
public:
    const TexturePtr & loadTexture(const FilePath &path, vector2i *size = 0);
    void freeTexture(u32 fsId, const c8 *fileName);
    void freeTexture(const FilePath &path);
    void freeAllTextures();
    void freeUnusedTextures();
private:
    typedef std::map<FilePath, std::pair<TexturePtr, vector2i> > TextureMap;
    TextureMap m_textures;
};

_vn_end

#endif
