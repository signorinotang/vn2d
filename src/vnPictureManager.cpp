//
//  vnPictureManager.cpp
//  vngfx
//
//  Created by Wayne on 13-12-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnPictureManager.h"
#include "vnTextureManager.h"
#include "vnFileManager.h"

_vn_begin

PictureManager::PictureManager()
: m_scale(1, 1) {
    
}

PictureManager::~PictureManager() {
    for (PictureSetInfoMap::iterator it = m_info.begin(); it != m_info.end(); ++it) {
        if (it->second) {
            delete it->second;
        }
    }
}

void PictureManager::setScale(const vector2f &scale) {
    m_scale = scale;
}

const vector2f & PictureManager::getScale() const {
    return m_scale;
}

Picture * PictureManager::createPicture(const c8 *name) {
    FilePath _file;
    c8 buf0[256];
    c8 buf1[128];
    if (sscanf(name, "%u:/%[^|]|%s", &_file.fsId, buf0, buf1) != 3) {
        return 0;
    }
    _file.fileName = buf0;
    return createPicture(_file, buf1);
}

Picture * PictureManager::createPicture(const FilePath &_file, const str8 &_name) {
    std::pair<PictureSetInfoMap::iterator, bool> ret = m_info.insert(std::make_pair(_file, (PictureSetInfo *)0));
    
    if (ret.second) {
        
        FilePath infofile = _file;
        infofile.fileName.append("/info");
        // TODO: create PictureSetInfo
    }
    
    // packed.
    if (ret.first->second) {
        PictureSetInfo::PictureMap::iterator it = ret.first->second->pictures.find(_name);
        if (it == ret.first->second->pictures.end()) {
            return 0;
        }
        Picture *pic = vnnew Picture();
        pic->texture = TextureManager::instance().loadTexture(*it->second.texture);
		pic->size = vector2f(it->second.rect.size()) / m_scale;
        if (!pic->texture.null()) {
            pic->buildTexcoord(pic->texture->size(), it->second.rect);
        }
        return pic;
    }
    
    // unpacked.
    FilePath file =_file;
    file.fileName.push_back('/');
    file.fileName.append(_name);
	vector2i size;
    TexturePtr tex = TextureManager::instance().loadTexture(file, &size);
    if (tex.null()) {
        return 0;
    }
    Picture *pic = vnnew Picture();
    pic->texture = tex;
	pic->size = vector2f(size) / m_scale;
    pic->buildTexcoord(pic->texture->size(), aabox2f(vector2f::kZero, size));
    return pic;
}

bool PictureManager::list(const FilePath &file, std::list<str8> &result) {
	std::pair<PictureSetInfoMap::iterator, bool> ret = m_info.insert(std::make_pair(file, (PictureSetInfo *)0));
    
    if (ret.second) {
        str8 infofile = file.fileName;
        infofile.append("/info");
        // TODO: create PictureSetInfo
    }
    
    // packed.
    if (ret.first->second) {
		result.clear();
		for (PictureSetInfo::PictureMap::iterator it = ret.first->second->pictures.begin(); it != ret.first->second->pictures.end(); ++it) {
			result.push_back(it->first);
		}
		return true;
	}
	
	// unpacked.
	return FileManager::instance().list(file, result);
}


_vn_end