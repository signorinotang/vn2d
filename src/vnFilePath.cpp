//
//  vnFilePath.cpp
//  vnbase
//
//  Created by Wayne on 13-12-9.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnFilePath.h"
#include <cstdio>
#include <sstream>

_vn_begin


FilePath::FilePath()
: fsId(0) {
    
}

FilePath::FilePath(u32 _fsId, const str8 &_fileName)
: fsId(_fsId)
, fileName(_fileName) {
    
}

FilePath::FilePath(u32 _fsId, const c8 *_fileName)
: fsId(_fsId)
, fileName(_fileName) {
    
}

FilePath::FilePath(const c8 *fullPath)
: fsId(0){
    vnassert(fullPath);
    int off = 0;
    if (sscanf(fullPath, "%u:/%n", &fsId, &off) && off) {
        fileName = fullPath + off;
    }
}

str8 & FilePath::getFullPath(str8 &fullPath) const {
    std::ostringstream oss;
    oss << fsId << ":/" << fileName;
    return fullPath = oss.str();
}

str8 FilePath::getFullPath() const {
    std::ostringstream oss;
    oss << fsId << ":/" << fileName;
    return oss.str();
}

bool FilePath::operator !=(const FilePath &rhs) const {
    return fsId != rhs.fsId || fileName != rhs.fileName;
}

bool FilePath::operator ==(const FilePath &rhs) const {
    return fsId == rhs.fsId && fileName == rhs.fileName;
}

bool FilePath::operator <(const FilePath &rhs) const {
    if (fsId < rhs.fsId) {
        return true;
    }
    if (fsId == rhs.fsId && fileName < rhs.fileName) {
        return true;
    }
    return false;
}

FilePath & FilePath::operator =(const FilePath &rhs) {
    fsId = rhs.fsId;
    fileName = rhs.fileName;
    return *this;
}

FilePath & FilePath::operator =(const c8 *fullPath) {
    vnassert(fullPath);
    int off = 0;
    if (sscanf(fullPath, "%u:/%n", &fsId, &off) && off) {
        fileName = fullPath + off;
    }
    return *this;
}



_vn_end