//
//  vnSceneNodeContainer.cpp
//  vn2d
//
//  Created by Wayne on 3/11/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSceneNodeContainer.h"
#include "vnSceneNode.h"

_vn_begin

SceneNodeContainer::~SceneNodeContainer() {
    
}

SceneNodeContainerDefault::~SceneNodeContainerDefault() {
    for (Nodes::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        (*it)->drop();
    }
}

void SceneNodeContainerDefault::add(SceneNode *node, bool grab) {
    vnassert(node);
    if (grab) {
        node->grab();
    }
    m_nodes.push_back(node);
}

void SceneNodeContainerDefault::remove(SceneNode *node) {
    vnassert(node);
    for (Nodes::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        if (*it == node) {
            m_nodes.erase(it);
            node->drop();
            break;
        }
    }
}

void SceneNodeContainerDefault::clear() {
    for (Nodes::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        (*it)->drop();
    }
    m_nodes.clear();
}

size_t SceneNodeContainerDefault::size() const {
    return m_nodes.size();
}

void SceneNodeContainerDefault::queryVisible(const aabox2f &view, std::vector<SceneNode *> &nodes) const {
    for (Nodes::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        (*it)->queryVisibleNodes(view, nodes);
    }
}

bool SceneNodeContainerDefault::accept(Visitor *visitor) const {
    for (Nodes::const_iterator it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        if (!visitor->visit(*it)) {
            return false;
        }
    }
    return true;
}

void SceneNodeContainerDefault::notifyPositionUpdated(SceneNode *node) {
    
}

_vn_end
