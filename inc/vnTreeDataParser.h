//
//  vnTreeDataParser.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnTreeDataParser_h
#define vn2d_vnTreeDataParser_h

#include "vnTreeData.h"
#include "vnFilePath.h"
#include "vnSingleton.h"

_vn_begin

class TreeDataScript;

class _vn_2d_api TreeDataParser : public Singleton<TreeDataParser> {
public:
    TreeDataParser();
    ~TreeDataParser();
    
    TreeDataNode * parse(const FilePath &file);
    
private:
    TreeDataScript *m_script;
};

_vn_end

#endif
