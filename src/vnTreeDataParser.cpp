//
//  vnTreeDataParser.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnTreeDataParser.h"
#include "lex/vnTreeDataScript.h"
#include "vnLog.h"

_vn_begin

TreeDataParser::TreeDataParser()
: m_script(vnnew TreeDataScript()) {
    
}

TreeDataParser::~TreeDataParser() {
    delete m_script;
}

TreeDataNode * TreeDataParser::parse(const FilePath &file) {
    TreeDataNode *node = m_script->parse(file);
    if (!node) {
        VN_LOG_ERROR(m_script->error());
    }
    return node;
}

_vn_end
